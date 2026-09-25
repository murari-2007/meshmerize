#include <cassert>

#include "core/types.h"
#include "maze/edge.h"
#include "maze/graph.h"
#include "maze/maze_map.h"
#include "maze/node.h"

int main()
{
    using namespace meshmerize;

    Graph graph;

    Node start;
    start.id = 0;
    start.x = 0.0;
    start.y = 0.0;
    start.is_start = true;

    Node junction;
    junction.id = 1;
    junction.x = 100.0;
    junction.y = 0.0;

    Node end;
    end.id = 2;
    end.x = 200.0;
    end.y = 0.0;
    end.is_end = true;

    assert(graph.addNode(start));
    assert(graph.addNode(junction));
    assert(graph.addNode(end));

    assert(graph.nodeCount() == 3);

    // Duplicate node must be rejected.
    assert(!graph.addNode(start));

    Edge edge_01;
    edge_01.from = 0;
    edge_01.to = 1;
    edge_01.distance = 100.0;
    edge_01.direction = Direction::East;

    Edge edge_12;
    edge_12.from = 1;
    edge_12.to = 2;
    edge_12.distance = 100.0;
    edge_12.direction = Direction::East;

    assert(graph.addEdge(edge_01));
    assert(graph.addEdge(edge_12));

    assert(graph.edgeCount() == 2);

    // Check node lookup.
    const Node* found = graph.getNode(1);

    assert(found != nullptr);
    assert(found->id == 1);
    assert(found->x == 100.0);

    // Check neighbors.
    auto start_neighbors = graph.getNeighbors(0);

    assert(start_neighbors.size() == 1);
    assert(start_neighbors[0] == 1);

    auto junction_neighbors = graph.getNeighbors(1);

    assert(junction_neighbors.size() == 2);

    // Invalid edge should be rejected.
    Edge invalid_edge;
    invalid_edge.from = 1;
    invalid_edge.to = 99;

    assert(!graph.addEdge(invalid_edge));

    // MazeMap.
    MazeMap maze;

    Node maze_start;
    maze_start.id = 10;
    maze_start.is_start = true;

    Node maze_end;
    maze_end.id = 11;
    maze_end.is_end = true;

    assert(maze.graph().addNode(maze_start));
    assert(maze.graph().addNode(maze_end));

    assert(maze.setStartNode(10));
    assert(maze.setEndNode(11));

    assert(maze.startNode() == 10);
    assert(maze.endNode() == 11);

    // Invalid start/end node should be rejected.
    assert(!maze.setStartNode(99));
    assert(!maze.setEndNode(99));

    return 0;
}