#include <cassert>
#include <cmath>
#include <vector>

#include "execution/recovery.h"
#include "maze/graph.h"
#include "planning/bfs.h"
#include "planning/dijkstra.h"
#include "simulator/virtual_robot.h"
#include "maze/maze_builder.h"
#include "maze/maze_map.h"

using namespace meshmerize;

static Graph buildBranchingGraph()
{
    Graph graph;

    assert(graph.addNode({0}));
    assert(graph.addNode({1}));
    assert(graph.addNode({2}));
    assert(graph.addNode({3}));
    assert(graph.addNode({4}));
    assert(graph.addNode({5}));
    assert(graph.addNode({6}));

    // Main route:
    // 0 -> 1 -> 3 -> 6
    //
    // Alternative branches:
    // 0 -> 2 -> 3
    // 1 -> 4 -> 5 -> 6

    assert(graph.addEdge({0, 1, 10.0}));
    assert(graph.addEdge({1, 3, 10.0}));
    assert(graph.addEdge({3, 6, 10.0}));

    assert(graph.addEdge({0, 2, 5.0}));
    assert(graph.addEdge({2, 3, 20.0}));

    assert(graph.addEdge({1, 4, 2.0}));
    assert(graph.addEdge({4, 5, 2.0}));
    assert(graph.addEdge({5, 6, 50.0}));

    return graph;
}

static void test_bfs_handles_cycles()
{
    Graph graph;

    for (int i = 0; i < 5; ++i) {
        assert(graph.addNode({i}));
    }

    assert(graph.addEdge({0, 1, 1.0}));
    assert(graph.addEdge({1, 2, 1.0}));
    assert(graph.addEdge({2, 3, 1.0}));
    assert(graph.addEdge({3, 0, 1.0}));
    assert(graph.addEdge({3, 4, 1.0}));

    const std::vector<int> path = bfs(graph, 0, 4);

    assert(!path.empty());
    assert(path.front() == 0);
    assert(path.back() == 4);
}

static void test_dijkstra_chooses_weighted_route()
{
    Graph graph = buildBranchingGraph();

    const std::vector<int> path =
        dijkstra(graph, 0, 6);

    assert(!path.empty());
    assert(path.front() == 0);
    assert(path.back() == 6);

    // Expected weighted shortest route:
    // 0 -> 1 -> 3 -> 6
    assert(path.size() == 4);
    assert(path[0] == 0);
    assert(path[1] == 1);
    assert(path[2] == 3);
    assert(path[3] == 6);
}

static void test_dijkstra_avoids_expensive_branch()
{
    Graph graph = buildBranchingGraph();

    const std::vector<int> path =
        dijkstra(graph, 0, 6);

    for (const int node : path) {
        assert(node != 2);
        assert(node != 4);
        assert(node != 5);
    }
}

static void test_dijkstra_unreachable_in_cyclic_graph()
{
    Graph graph;

    assert(graph.addNode({0}));
    assert(graph.addNode({1}));
    assert(graph.addNode({2}));
    assert(graph.addNode({99}));

    assert(graph.addEdge({0, 1, 1.0}));
    assert(graph.addEdge({1, 2, 1.0}));
    assert(graph.addEdge({2, 0, 1.0}));

    const std::vector<int> path =
        dijkstra(graph, 0, 99);

    assert(path.empty());
}

static void test_graph_rejects_invalid_edges()
{
    Graph graph;

    assert(graph.addNode({0}));
    assert(graph.addNode({1}));

    // Valid edge.
    assert(graph.addEdge({0, 1, 10.0}));

    // Duplicate undirected edge must be rejected.
    assert(!graph.addEdge({1, 0, 10.0}));

    // Edge containing an unknown node must be rejected.
    assert(!graph.addEdge({1, 99, 10.0}));

    assert(graph.edgeCount() == 1);
}

static void test_graph_exploration_is_direction_independent()
{
    Graph graph;

    assert(graph.addNode({0}));
    assert(graph.addNode({1}));
    assert(graph.addEdge({0, 1, 10.0}));

    assert(!graph.edges()[0].explored);

    assert(graph.markEdgeExplored(1, 0));

    assert(graph.edges()[0].explored);
}

static void test_virtual_robot_uturn_and_backtracking()
{
    MazeMap maze;
    MazeBuilder builder(maze);

    assert(builder.addStartNode(0, 0.0, 0.0));
    assert(builder.addDiscoveredNode(1, 0.0, 100.0));
    assert(builder.addDiscoveredNode(2, 0.0, 200.0));

    assert(builder.connectNodes(
        0,
        1,
        Direction::North,
        100.0
    ));

    assert(builder.connectNodes(
        1,
        2,
        Direction::North,
        100.0
    ));

    assert(maze.setEndNode(2));

    VirtualRobot robot(maze);

    assert(robot.state().node_id == 0);
    assert(robot.state().direction == Direction::North);

    assert(robot.moveForward(100.0));
    assert(robot.state().node_id == 1);

    assert(robot.moveForward(100.0));
    assert(robot.state().node_id == 2);

    assert(robot.turnAround());
    assert(robot.state().direction == Direction::South);

    assert(robot.moveForward(100.0));
    assert(robot.state().node_id == 1);

    assert(robot.moveForward(100.0));
    assert(robot.state().node_id == 0);
}

static void test_virtual_robot_cannot_move_without_edge()
{
    MazeMap maze;
    MazeBuilder builder(maze);

    assert(builder.addStartNode(0, 0.0, 0.0));
    assert(builder.addDiscoveredNode(1, 0.0, 100.0));

    assert(maze.setEndNode(1));

    VirtualRobot robot(maze);

    assert(!robot.moveForward(100.0));
    assert(robot.state().node_id == 0);
}

static void test_recovery_sequence()
{
    RecoveryManager manager;

    manager.reportLineLost();

    assert(manager.state() ==
           RecoveryState::LineLost);

    assert(manager.action() ==
           RecoveryAction::RecoverLine);

    manager.reset();

    manager.reportTurnFailed();

    assert(manager.state() ==
           RecoveryState::TurnFailed);

    assert(manager.action() ==
           RecoveryAction::RetryTurn);

    manager.reset();

    manager.reportTimeout();

    assert(manager.state() ==
           RecoveryState::Timeout);

    assert(manager.action() ==
           RecoveryAction::Stop);

    assert(manager.faulted());
}

static void test_terminal_recovery_states_are_stable()
{
    RecoveryManager manager;

    manager.reportEndDetected();

    assert(manager.completed());

    manager.reportLineLost();
    manager.reportTurnFailed();
    manager.reportTimeout();

    assert(manager.state() ==
           RecoveryState::EndDetected);

    assert(manager.completed());
}

static void test_zero_distance_edge()
{
    Graph graph;

    assert(graph.addNode({0}));
    assert(graph.addNode({1}));

    assert(graph.addEdge({0, 1, 0.0}));

    const std::vector<int> path =
        dijkstra(graph, 0, 1);

    assert(path.size() == 2);
    assert(path[0] == 0);
    assert(path[1] == 1);
}

int main()
{
    test_bfs_handles_cycles();
    test_dijkstra_chooses_weighted_route();
    test_dijkstra_avoids_expensive_branch();
    test_dijkstra_unreachable_in_cyclic_graph();
    test_graph_rejects_invalid_edges();
    test_graph_exploration_is_direction_independent();
    test_virtual_robot_uturn_and_backtracking();
    test_virtual_robot_cannot_move_without_edge();
    test_recovery_sequence();
    test_terminal_recovery_states_are_stable();
    test_zero_distance_edge();

    return 0;
}