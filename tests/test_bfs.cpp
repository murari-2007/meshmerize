#include <cassert>
#include <vector>

#include "maze/edge.h"
#include "maze/graph.h"
#include "maze/node.h"
#include "planning/bfs.h"

namespace {

meshmerize::Graph createLinearGraph()
{
    using namespace meshmerize;

    Graph graph;

    Node node0;
    node0.id = 0;

    Node node1;
    node1.id = 1;

    Node node2;
    node2.id = 2;

    Node node3;
    node3.id = 3;

    graph.addNode(node0);
    graph.addNode(node1);
    graph.addNode(node2);
    graph.addNode(node3);

    Edge edge01;
    edge01.from = 0;
    edge01.to = 1;

    Edge edge12;
    edge12.from = 1;
    edge12.to = 2;

    Edge edge23;
    edge23.from = 2;
    edge23.to = 3;

    graph.addEdge(edge01);
    graph.addEdge(edge12);
    graph.addEdge(edge23);

    return graph;
}

} // namespace

int main()
{
    using namespace meshmerize;

    // --------------------------------------------------
    // Test 1: Simple linear path
    // --------------------------------------------------

    Graph graph = createLinearGraph();

    std::vector<int> path = bfs(
        graph,
        0,
        3
    );

    assert(
        path ==
        std::vector<int>({
            0,
            1,
            2,
            3
        })
    );

    // --------------------------------------------------
    // Test 2: Start equals goal
    // --------------------------------------------------

    path = bfs(
        graph,
        2,
        2
    );

    assert(
        path ==
        std::vector<int>({
            2
        })
    );

    // --------------------------------------------------
    // Test 3: Reverse direction
    //
    // Graph edges are undirected.
    // --------------------------------------------------

    path = bfs(
        graph,
        3,
        0
    );

    assert(
        path ==
        std::vector<int>({
            3,
            2,
            1,
            0
        })
    );

    // --------------------------------------------------
    // Test 4: Invalid start node
    // --------------------------------------------------

    path = bfs(
        graph,
        99,
        3
    );

    assert(path.empty());

    // --------------------------------------------------
    // Test 5: Invalid goal node
    // --------------------------------------------------

    path = bfs(
        graph,
        0,
        99
    );

    assert(path.empty());

    // --------------------------------------------------
    // Test 6: Unreachable goal
    // --------------------------------------------------

    Node isolated;
    isolated.id = 10;

    assert(graph.addNode(isolated));

    path = bfs(
        graph,
        0,
        10
    );

    assert(path.empty());

    // --------------------------------------------------
    // Test 7: BFS chooses shortest path
    //
    //        1
    //       / \
    //      0   3
    //       \ /
    //        2
    //
    // Both paths have equal length.
    // --------------------------------------------------

    Graph branching;

    for (int id = 0; id <= 3; ++id) {
        Node node;
        node.id = id;

        assert(branching.addNode(node));
    }

    Edge edge01;
    edge01.from = 0;
    edge01.to = 1;

    Edge edge13;
    edge13.from = 1;
    edge13.to = 3;

    Edge edge02;
    edge02.from = 0;
    edge02.to = 2;

    Edge edge23;
    edge23.from = 2;
    edge23.to = 3;

    assert(branching.addEdge(edge01));
    assert(branching.addEdge(edge13));
    assert(branching.addEdge(edge02));
    assert(branching.addEdge(edge23));

    path = bfs(
        branching,
        0,
        3
    );

    assert(path.size() == 3);
    assert(path.front() == 0);
    assert(path.back() == 3);

    // --------------------------------------------------
    // Test 8: BFS prefers fewer edges
    //
    // 0 ---- 3
    //  \
    //   1 ---- 2 ---- 3
    //
    // Expected:
    // 0 -> 3
    // --------------------------------------------------

    Graph shortest;

    for (int id = 0; id <= 3; ++id) {
        Node node;
        node.id = id;

        assert(shortest.addNode(node));
    }

    Edge direct;
    direct.from = 0;
    direct.to = 3;

    Edge long01;
    long01.from = 0;
    long01.to = 1;

    Edge long12;
    long12.from = 1;
    long12.to = 2;

    Edge long23;
    long23.from = 2;
    long23.to = 3;

    assert(shortest.addEdge(direct));
    assert(shortest.addEdge(long01));
    assert(shortest.addEdge(long12));
    assert(shortest.addEdge(long23));

    path = bfs(
        shortest,
        0,
        3
    );

    assert(
        path ==
        std::vector<int>({
            0,
            3
        })
    );

    return 0;
}