#include "planning/bfs.h"
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace meshmerize {

std::vector<int> bfs(
    const Graph& graph,
    int start,
    int goal
)
{
    // --------------------------------------------------
    // Validate start and goal
    // --------------------------------------------------

    if (!graph.containsNode(start) ||
        !graph.containsNode(goal)) {
        return {};
    }

    // --------------------------------------------------
    // Start and goal are the same node
    // --------------------------------------------------

    if (start == goal) {
        return {start};
    }

    // --------------------------------------------------
    // BFS data structures
    // --------------------------------------------------

    std::queue<int> queue;

    std::unordered_set<int> visited;

    std::unordered_map<int, int> parent;

    // --------------------------------------------------
    // Initialize BFS
    // --------------------------------------------------

    queue.push(start);
    visited.insert(start);

    // --------------------------------------------------
    // BFS traversal
    // --------------------------------------------------

    while (!queue.empty()) {

        const int current = queue.front();
        queue.pop();

        const std::vector<int> neighbors =
            graph.getNeighbors(current);

        for (const int neighbor : neighbors) {

            // Ignore nodes already visited.
            if (visited.find(neighbor) != visited.end()) {
                continue;
            }
            visited.insert(neighbor);

            parent[neighbor] = current;

            // Goal found.
            if (neighbor == goal) {

                std::vector<int> path;

                int node = goal;

                while (node != start) {
                    path.push_back(node);
                    node = parent[node];
                }

                path.push_back(start);

                // Reverse:
                // goal -> ... -> start
                // becomes:
                // start -> ... -> goal
                std::reverse(
                    path.begin(),
                    path.end()
                );

                return path;
            }

            queue.push(neighbor);
        }
    }

    // --------------------------------------------------
    // Goal is unreachable
    // --------------------------------------------------

    return {};
}

} // namespace meshmerize