#pragma once

#include "planning/route.h"

namespace meshmerize {

class PathOptimizer {
public:
    Route optimize(const Route& route) const;
};

} // namespace meshmerize