#pragma once

#include "core/types.h"
#include "maze/maze_map.h"

namespace meshmerize {

class MazeBuilder {
public:
    explicit MazeBuilder(MazeMap& maze);

    bool addStartNode(
        int node_id,
        double x,
        double y
    );

    bool addDiscoveredNode(
        int node_id,
        double x,
        double y
    );

    bool connectNodes(
        int from,
        int to,
        Direction direction,
        double distance
    );

    bool markEdgeExplored(
        int from,
        int to
    );

    bool hasNode(int node_id) const;

    bool hasConnection(
        int from,
        int to
    ) const;

private:
    MazeMap& maze_;
};

} // namespace meshmerize