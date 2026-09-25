#pragma once

#include <cstddef>
#include <vector>

#include "maze/edge.h"
#include "maze/node.h"

namespace meshmerize {

class Graph {
public:
    bool addNode(const Node& node);

    bool addEdge(const Edge& edge);

    bool containsNode(int node_id) const;

    const Node* getNode(int node_id) const;

    const std::vector<Node>& nodes() const;

    const std::vector<Edge>& edges() const;

    std::vector<int> getNeighbors(int node_id) const;

    std::size_t nodeCount() const;

    std::size_t edgeCount() const;

private:
    std::vector<Node> nodes_;
    std::vector<Edge> edges_;
};

} // namespace meshmerize