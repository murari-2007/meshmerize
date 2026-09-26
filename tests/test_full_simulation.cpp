#include <cassert>
#include <vector>

#include "maze/maze_builder.h"
#include "maze/maze_map.h"
#include "planning/dijkstra.h"
#include "simulator/virtual_robot.h"

using namespace meshmerize;

static MazeMap buildTestMaze()
{
    MazeMap maze;
    MazeBuilder builder(maze);

    // --------------------------------------------------
    // Maze
    //
    //             2
    //             |
    //             |
    //       1 ----3---- 4
    //       |
    //       |
    //       0
    //
    // Start = 0
    // End   = 4
    //
    // There is also a longer path:
    //
    // 0 -> 1 -> 2 -> 3 -> 4
    //
    // and a shorter path:
    //
    // 0 -> 1 -> 3 -> 4
    // --------------------------------------------------

    assert(builder.addStartNode(
        0,
        0.0,
        0.0
    ));

    assert(builder.addDiscoveredNode(
        1,
        0.0,
        100.0
    ));

    assert(builder.addDiscoveredNode(
        2,
        0.0,
        200.0
    ));

    assert(builder.addDiscoveredNode(
        3,
        100.0,
        100.0
    ));

    assert(builder.addDiscoveredNode(
        4,
        200.0,
        100.0
    ));

    // Start -> 1
    assert(builder.connectNodes(
        0,
        1,
        Direction::North,
        100.0
    ));

    // 1 -> 2
    assert(builder.connectNodes(
        1,
        2,
        Direction::North,
        100.0
    ));

    // 2 -> 3
    assert(builder.connectNodes(
        2,
        3,
        Direction::East,
        141.0
    ));

    // 1 -> 3
    assert(builder.connectNodes(
        1,
        3,
        Direction::East,
        100.0
    ));

    // 3 -> 4
    assert(builder.connectNodes(
        3,
        4,
        Direction::East,
        100.0
    ));

    assert(maze.setEndNode(4));

    return maze;
}

static void test_shortest_path()
{
    MazeMap maze = buildTestMaze();

    const std::vector<int> path =
        dijkstra(
            maze.graph(),
            maze.startNode(),
            maze.endNode()
        );

    assert(path.size() == 4);

    assert(path[0] == 0);
    assert(path[1] == 1);
    assert(path[2] == 3);
    assert(path[3] == 4);
}

static void test_virtual_robot_executes_path()
{
    MazeMap maze = buildTestMaze();

    VirtualRobot robot(maze);

    // VirtualRobot starts facing North.
    assert(robot.state().node_id == 0);
    assert(robot.state().direction == Direction::North);

    // --------------------------------------------------
    // 0 -> 1
    // Already facing North.
    // --------------------------------------------------

    assert(robot.moveForward(100.0));

    assert(robot.state().node_id == 1);

    // --------------------------------------------------
    // 1 -> 3
    // Turn East.
    // --------------------------------------------------

    assert(robot.turnRight());

    assert(
        robot.state().direction ==
        Direction::East
    );

    assert(robot.moveForward(100.0));

    assert(robot.state().node_id == 3);

    // --------------------------------------------------
    // 3 -> 4
    // Continue East.
    // --------------------------------------------------

    assert(robot.moveForward(100.0));

    assert(robot.state().node_id == 4);

    // End reached.
    assert(
        robot.state().node_id ==
        maze.endNode()
    );

    assert(robot.state().x == 200.0);
    assert(robot.state().y == 100.0);
}

static void test_longer_path_is_not_selected()
{
    MazeMap maze = buildTestMaze();

    const std::vector<int> path =
        dijkstra(
            maze.graph(),
            maze.startNode(),
            maze.endNode()
        );

    // The longer route would contain:
    //
    // 0 -> 1 -> 2 -> 3 -> 4
    //
    // The selected route must not contain node 2.

    for (const int node : path) {
        assert(node != 2);
    }
}

int main()
{
    test_shortest_path();

    test_virtual_robot_executes_path();

    test_longer_path_is_not_selected();

    return 0;
}