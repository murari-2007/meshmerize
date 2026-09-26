#include "exploration/junction_decision.h"

namespace meshmerize {

JunctionDecision chooseSLRB(
    const JunctionObservation& observation,
    bool straight_explored,
    bool left_explored,
    bool right_explored
)
{
    JunctionDecision decision;

    // S — Straight
    if (observation.straight && !straight_explored) {
        decision.direction = RelativeDirection::Straight;
        return decision;
    }

    // L — Left
    if (observation.left && !left_explored) {
        decision.direction = RelativeDirection::Left;
        return decision;
    }

    // R — Right
    if (observation.right && !right_explored) {
        decision.direction = RelativeDirection::Right;
        return decision;
    }

    // B — Backtrack
    decision.direction = RelativeDirection::Back;
    decision.should_backtrack = true;

    return decision;
}

} // namespace meshmerize