#pragma once

#include <vector>

#include "core/types.h"
#include "maze/graph.h"
#include "planning/route.h"

namespace meshmerize {

class RouteBuilder {
public:
    Route build(
        const Graph& graph,
        const std::vector<int>& node_path,
        Direction initial_direction
    ) const;

private:
    Direction edgeDirection(
        const Graph& graph,
        int from,
        int to
    ) const;
};

} // namespace meshmerize
