#include <cassert>
#include <vector>

#include "maze/graph.h"
#include "planning/dijkstra.h"
#include "planning/path_optimizer.h"
#include "planning/route_builder.h"

using namespace meshmerize;

static Graph buildMaze()
{
    Graph graph;

    /*
     * Maze:
     *
     *             2
     *             |
     *             |
     *             1 ---- 3 ---- 4
     *             |
     *             |
     *             0
     *
     * Start = 0
     * End   = 4
     *
     * Shortest path:
     *
     * 0 -> 1 -> 3 -> 4
     */

    assert(graph.addNode({0, 0.0, 0.0}));
    assert(graph.addNode({1, 0.0, 100.0}));
    assert(graph.addNode({2, 0.0, 200.0}));
    assert(graph.addNode({3, 100.0, 100.0}));
    assert(graph.addNode({4, 200.0, 100.0}));

    assert(
        graph.addEdge({
            0,
            1,
            100.0,
            Direction::North
        })
    );

    assert(
        graph.addEdge({
            1,
            2,
            100.0,
            Direction::North
        })
    );

    assert(
        graph.addEdge({
            1,
            3,
            100.0,
            Direction::East
        })
    );

    assert(
        graph.addEdge({
            3,
            4,
            100.0,
            Direction::East
        })
    );

    return graph;
}

static void test_shortest_node_path()
{
    const Graph graph =
        buildMaze();

    const std::vector<int> path =
        dijkstra(
            graph,
            0,
            4
        );

    assert(path.size() == 4);

    assert(path[0] == 0);
    assert(path[1] == 1);
    assert(path[2] == 3);
    assert(path[3] == 4);
}

static void test_build_route()
{
    const Graph graph =
        buildMaze();

    const std::vector<int> path =
        dijkstra(
            graph,
            0,
            4
        );

    RouteBuilder builder;

    const Route route =
        builder.build(
            graph,
            path,
            Direction::North
        );

    /*
     * 0 -> 1
     *
     * Already facing North:
     * Forward
     *
     * 1 -> 3
     *
     * North -> East:
     * Right
     * Forward
     *
     * 3 -> 4
     *
     * Already facing East:
     * Forward
     */

    assert(route.size() == 4);

    assert(
        route.actions[0] ==
        Action::Forward
    );

    assert(
        route.actions[1] ==
        Action::Right
    );

    assert(
        route.actions[2] ==
        Action::Forward
    );

    assert(
        route.actions[3] ==
        Action::Forward
    );
}

static void test_path_optimizer()
{
    const Graph graph =
        buildMaze();

    const std::vector<int> path =
        dijkstra(
            graph,
            0,
            4
        );

    RouteBuilder builder;

    const Route route =
        builder.build(
            graph,
            path,
            Direction::North
        );

    PathOptimizer optimizer;

    const Route optimized =
        optimizer.optimize(route);

    assert(optimized.size() == 5);

    assert(
        optimized.actions[0] ==
        Action::Forward
    );

    assert(
        optimized.actions[1] ==
        Action::Right
    );

    assert(
        optimized.actions[2] ==
        Action::Forward
    );

    assert(
        optimized.actions[3] ==
        Action::Forward
    );

    assert(
        optimized.actions[4] ==
        Action::Stop
    );
}

static void test_reverse_edge_direction()
{
    Graph graph;

    graph.addNode({0});
    graph.addNode({1});

    /*
     * Edge is stored:
     *
     * 0 -> 1 = North
     *
     * But route goes:
     *
     * 1 -> 0 = South
     */
    assert(
        graph.addEdge({
            0,
            1,
            100.0,
            Direction::North
        })
    );

    RouteBuilder builder;

    const Route route =
        builder.build(
            graph,
            {1, 0},
            Direction::South
        );

    /*
     * Already facing South.
     */
    assert(route.size() == 1);

    assert(
        route.actions[0] ==
        Action::Forward
    );
}

static void test_left_turn()
{
    Graph graph;

    graph.addNode({0});
    graph.addNode({1});

    /*
     * Robot starts facing North.
     *
     * Target edge is West.
     *
     * Therefore:
     *
     * Left + Forward
     */
    assert(
        graph.addEdge({
            0,
            1,
            100.0,
            Direction::West
        })
    );

    RouteBuilder builder;

    const Route route =
        builder.build(
            graph,
            {0, 1},
            Direction::North
        );

    assert(route.size() == 2);

    assert(
        route.actions[0] ==
        Action::Left
    );

    assert(
        route.actions[1] ==
        Action::Forward
    );
}

static void test_uturn()
{
    Graph graph;

    graph.addNode({0});
    graph.addNode({1});

    /*
     * Robot starts facing North.
     *
     * Target edge is South.
     *
     * Therefore:
     *
     * UTurn + Forward
     */
    assert(
        graph.addEdge({
            0,
            1,
            100.0,
            Direction::South
        })
    );

    RouteBuilder builder;

    const Route route =
        builder.build(
            graph,
            {0, 1},
            Direction::North
        );

    assert(route.size() == 2);

    assert(
        route.actions[0] ==
        Action::UTurn
    );

    assert(
        route.actions[1] ==
        Action::Forward
    );
}

static void test_empty_path()
{
    Graph graph;

    graph.addNode({0});

    RouteBuilder builder;

    const Route route =
        builder.build(
            graph,
            {},
            Direction::North
        );

    assert(route.empty());
}

static void test_single_node_path()
{
    Graph graph;

    graph.addNode({0});

    RouteBuilder builder;

    const Route route =
        builder.build(
            graph,
            {0},
            Direction::North
        );

    assert(route.empty());
}

int main()
{
    test_shortest_node_path();
    test_build_route();
    test_path_optimizer();
    test_reverse_edge_direction();
    test_left_turn();
    test_uturn();
    test_empty_path();
    test_single_node_path();

    return 0;
}
