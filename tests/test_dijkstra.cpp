#include <cassert>
#include <cmath>
#include <vector>

#include "maze/edge.h"
#include "maze/graph.h"
#include "maze/node.h"
#include "planning/dijkstra.h"

using namespace meshmerize;

static Graph createWeightedGraph()
{
    Graph graph;

    graph.addNode({0, 0.0, 0.0});
    graph.addNode({1, 1.0, 0.0});
    graph.addNode({2, 2.0, 0.0});
    graph.addNode({3, 3.0, 0.0});

    graph.addEdge({0, 1, 100.0});
    graph.addEdge({1, 3, 100.0});

    graph.addEdge({0, 2, 20.0});
    graph.addEdge({2, 3, 20.0});

    return graph;
}

static void test_weighted_shortest_path()
{
    Graph graph = createWeightedGraph();

    const std::vector<int> path =
        dijkstra(graph, 0, 3);

    assert(path.size() == 3);

    assert(path[0] == 0);
    assert(path[1] == 2);
    assert(path[2] == 3);
}

static void test_direct_expensive_edge_vs_shorter_route()
{
    Graph graph;

    graph.addNode({0});
    graph.addNode({1});
    graph.addNode({2});

    graph.addEdge({0, 2, 100.0});
    graph.addEdge({0, 1, 10.0});
    graph.addEdge({1, 2, 10.0});

    const std::vector<int> path =
        dijkstra(graph, 0, 2);

    assert(path.size() == 3);

    assert(path[0] == 0);
    assert(path[1] == 1);
    assert(path[2] == 2);
}

static void test_start_equals_goal()
{
    Graph graph;

    graph.addNode({0});

    const std::vector<int> path =
        dijkstra(graph, 0, 0);

    assert(path.size() == 1);
    assert(path[0] == 0);
}

static void test_invalid_start()
{
    Graph graph;

    graph.addNode({0});

    const std::vector<int> path =
        dijkstra(graph, 99, 0);

    assert(path.empty());
}

static void test_invalid_goal()
{
    Graph graph;

    graph.addNode({0});

    const std::vector<int> path =
        dijkstra(graph, 0, 99);

    assert(path.empty());
}

static void test_unreachable_goal()
{
    Graph graph;

    graph.addNode({0});
    graph.addNode({1});

    const std::vector<int> path =
        dijkstra(graph, 0, 1);

    assert(path.empty());
}

static void test_reverse_path()
{
    Graph graph;

    graph.addNode({0});
    graph.addNode({1});
    graph.addNode({2});

    graph.addEdge({0, 1, 5.0});
    graph.addEdge({1, 2, 7.0});

    const std::vector<int> path =
        dijkstra(graph, 2, 0);

    assert(path.size() == 3);

    assert(path[0] == 2);
    assert(path[1] == 1);
    assert(path[2] == 0);
}

int main()
{
    test_weighted_shortest_path();
    test_direct_expensive_edge_vs_shorter_route();
    test_start_equals_goal();
    test_invalid_start();
    test_invalid_goal();
    test_unreachable_goal();
    test_reverse_path();

    return 0;
}