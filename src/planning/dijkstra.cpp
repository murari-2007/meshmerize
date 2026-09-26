#include "planning/dijkstra.h"

#include <algorithm>
#include <limits>
#include <queue>
#include <unordered_map>

namespace meshmerize {

std::vector<int> dijkstra(
    const Graph& graph,
    int start,
    int goal
) {
    if (!graph.containsNode(start) ||
        !graph.containsNode(goal)) {
        return {};
    }

    if (start == goal) {
        return {start};
    }

    using QueueEntry = std::pair<double, int>;

    const double infinity = std::numeric_limits<double>::infinity();

    std::unordered_map<int, double> distance;
    std::unordered_map<int, int> parent;

    for (const Node& node : graph.nodes()) {
        distance[node.id] = infinity;
    }

    distance[start] = 0.0;

    std::priority_queue<
        QueueEntry,
        std::vector<QueueEntry>,
        std::greater<QueueEntry>
    > queue;

    queue.push({0.0, start});

    while (!queue.empty()) {
        const auto [current_distance, current] = queue.top();
        queue.pop();

        if (current_distance > distance[current]) {
            continue;
        }

        if (current == goal) {
            break;
        }

        for (const int neighbor : graph.getNeighbors(current)) {
            const Edge* edge = graph.getEdge(current, neighbor);

            if (edge == nullptr) {
                continue;
            }

            const double new_distance =
                current_distance + edge->distance;

            if (new_distance < distance[neighbor]) {
                distance[neighbor] = new_distance;
                parent[neighbor] = current;

                queue.push({
                    new_distance,
                    neighbor
                });
            }
        }
    }

    if (distance[goal] == infinity) {
        return {};
    }

    std::vector<int> path;

    int current = goal;

    while (current != start) {
        path.push_back(current);
        current = parent[current];
    }

    path.push_back(start);

    std::reverse(path.begin(), path.end());

    return path;
}

} // namespace meshmerize