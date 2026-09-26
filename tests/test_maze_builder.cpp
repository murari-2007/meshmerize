#include <cassert>

#include "maze/maze_builder.h"

int main()
{
    using namespace meshmerize;

    MazeMap maze;
    MazeBuilder builder(maze);

    // --------------------------------------------------
    // Add start node
    // --------------------------------------------------

    assert(
        builder.addStartNode(
            0,
            0.0,
            0.0
        )
    );

    assert(builder.hasNode(0));
    assert(maze.startNode() == 0);

    // --------------------------------------------------
    // Add discovered node
    // --------------------------------------------------

    assert(
        builder.addDiscoveredNode(
            1,
            1.0,
            0.0
        )
    );

    assert(builder.hasNode(1));
    assert(maze.graph().nodeCount() == 2);

    // --------------------------------------------------
    // Connect nodes
    // --------------------------------------------------

    assert(
        builder.connectNodes(
            0,
            1,
            Direction::East,
            1.0
        )
    );

    assert(builder.hasConnection(0, 1));
    assert(builder.hasConnection(1, 0));

    assert(maze.graph().edgeCount() == 1);

    // --------------------------------------------------
    // Duplicate connection rejected
    // --------------------------------------------------

    assert(
        !builder.connectNodes(
            1,
            0,
            Direction::West,
            1.0
        )
    );

    assert(maze.graph().edgeCount() == 1);

    // --------------------------------------------------
    // Mark connection explored
    // --------------------------------------------------

    assert(
        builder.markEdgeExplored(0, 1)
    );

    const Edge* edge = maze.graph().getEdge(0, 1);

    assert(edge != nullptr);
    assert(edge->explored);

    // --------------------------------------------------
    // Invalid connection rejected
    // --------------------------------------------------

    assert(
        !builder.connectNodes(
            0,
            99,
            Direction::North,
            1.0
        )
    );

    return 0;
}