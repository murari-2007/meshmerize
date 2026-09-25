#pragma once

#include "core/types.h"

namespace meshmerize {

struct Edge {
    int from = -1;
    int to = -1;

    double distance = 0.0;

    Direction direction = Direction::North;

    bool explored = false;
};

} // namespace meshmerize