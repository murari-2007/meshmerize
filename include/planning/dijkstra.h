#pragma once

#include <vector>

#include "maze/graph.h"

namespace meshmerize {

std::vector<int> dijkstra(
    const Graph& graph,
    int start,
    int goal
);

} // namespace meshmerize