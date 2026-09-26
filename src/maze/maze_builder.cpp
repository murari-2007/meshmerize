#include "maze/maze_builder.h"

namespace meshmerize {

MazeBuilder::MazeBuilder(MazeMap& maze)
    : maze_(maze)
{
}

bool MazeBuilder::addStartNode(
    int node_id,
    double x,
    double y
)
{
    Node node;

    node.id = node_id;
    node.x = x;
    node.y = y;
    node.is_start = true;

    if (!maze_.graph().addNode(node)) {
        return false;
    }

    return maze_.setStartNode(node_id);
}

bool MazeBuilder::addDiscoveredNode(
    int node_id,
    double x,
    double y
)
{
    Node node;

    node.id = node_id;
    node.x = x;
    node.y = y;

    return maze_.graph().addNode(node);
}

bool MazeBuilder::connectNodes(
    int from,
    int to,
    Direction direction,
    double distance
)
{
    if (!maze_.graph().containsNode(from) ||
        !maze_.graph().containsNode(to)) {
        return false;
    }

    Edge edge;

    edge.from = from;
    edge.to = to;
    edge.distance = distance;
    edge.direction = direction;
    edge.explored = false;

    return maze_.graph().addEdge(edge);
}

bool MazeBuilder::markEdgeExplored(
    int from,
    int to
)
{
    return maze_.graph().markEdgeExplored(from, to);
}

bool MazeBuilder::hasNode(int node_id) const
{
    return maze_.graph().containsNode(node_id);
}

bool MazeBuilder::hasConnection(
    int from,
    int to
) const
{
    return maze_.graph().containsEdge(from, to);
}

} // namespace meshmerize