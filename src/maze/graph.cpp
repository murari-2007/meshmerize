#include "maze/graph.h"

namespace meshmerize {

bool Graph::addNode(const Node& node)
{
    if (containsNode(node.id)) {
        return false;
    }

    nodes_.push_back(node);
    return true;
}

bool Graph::addEdge(const Edge& edge)
{
    if (!containsNode(edge.from) ||
        !containsNode(edge.to)) {
        return false;
    }

    if (containsEdge(edge.from, edge.to)) {
        return false;
    }

    edges_.push_back(edge);
    return true;
}

bool Graph::containsNode(int node_id) const
{
    for (const Node& node : nodes_) {
        if (node.id == node_id) {
            return true;
        }
    }

    return false;
}

const Node* Graph::getNode(int node_id) const
{
    for (const Node& node : nodes_) {
        if (node.id == node_id) {
            return &node;
        }
    }

    return nullptr;
}

const Edge* Graph::getEdge(int from, int to) const
{
    for (const Edge& edge : edges_) {
        if ((edge.from == from && edge.to == to) ||
            (edge.from == to && edge.to == from)) {
            return &edge;
        }
    }

    return nullptr;
}

bool Graph::containsEdge(int from, int to) const
{
    return getEdge(from, to) != nullptr;
}

const std::vector<Node>& Graph::nodes() const
{
    return nodes_;
}

const std::vector<Edge>& Graph::edges() const
{
    return edges_;
}

std::vector<int> Graph::getNeighbors(int node_id) const
{
    std::vector<int> neighbors;

    for (const Edge& edge : edges_) {
        if (edge.from == node_id) {
            neighbors.push_back(edge.to);
        }
        else if (edge.to == node_id) {
            neighbors.push_back(edge.from);
        }
    }

    return neighbors;
}

std::size_t Graph::nodeCount() const
{
    return nodes_.size();
}

std::size_t Graph::edgeCount() const
{
    return edges_.size();
}

bool Graph::markEdgeExplored(int from, int to)
{
    for (Edge& edge : edges_) {
        if ((edge.from == from && edge.to == to) ||
            (edge.from == to && edge.to == from)) {
            edge.explored = true;
            return true;
        }
    }

    return false;
}

} // namespace meshmerize