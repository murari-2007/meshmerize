#pragma once

#include "maze/graph.h"

namespace meshmerize {

class MazeMap {
public:
    Graph& graph();

    const Graph& graph() const;

    bool setStartNode(int node_id);

    bool setEndNode(int node_id);

    int startNode() const;

    int endNode() const;

private:
    Graph graph_;

    int start_node_id_ = -1;
    int end_node_id_ = -1;
};

} // namespace meshmerize