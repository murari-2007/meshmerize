#pragma once

#include "core/types.h"
#include "perception/junction_detector.h"

namespace meshmerize {

struct JunctionDecision {
    RelativeDirection direction = RelativeDirection::Back;
    bool should_backtrack = false;
};

JunctionDecision chooseSLRB(
    const JunctionObservation& observation,
    bool straight_explored,
    bool left_explored,
    bool right_explored
);

} // namespace meshmerize