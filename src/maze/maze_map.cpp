#include "maze/maze_map.h"

namespace meshmerize {

Graph& MazeMap::graph()
{
    return graph_;
}

const Graph& MazeMap::graph() const
{
    return graph_;
}

bool MazeMap::setStartNode(int node_id)
{
    if (!graph_.containsNode(node_id)) {
        return false;
    }

    start_node_id_ = node_id;
    return true;
}

bool MazeMap::setEndNode(int node_id)
{
    if (!graph_.containsNode(node_id)) {
        return false;
    }

    end_node_id_ = node_id;
    return true;
}

int MazeMap::startNode() const
{
    return start_node_id_;
}

int MazeMap::endNode() const
{
    return end_node_id_;
}

} // namespace meshmerize