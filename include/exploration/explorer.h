#pragma once

#include <cstddef>

#include "core/types.h"
#include "exploration/exploration_state.h"
#include "exploration/junction_decision.h"
#include "perception/junction_detector.h"

namespace meshmerize {

class Explorer {
public:
    void reset();

    JunctionDecision chooseDirection(
        const JunctionObservation& observation,
        bool straight_explored,
        bool left_explored,
        bool right_explored
    ) const;

    bool enterBranch(
        int current_node,
        RelativeDirection direction
    );

    bool backtrack(
        ExplorationFrame& frame
    );

    void updateHeading(
        RelativeDirection direction
    );

    Direction heading() const;

    std::size_t depth() const;

private:
    ExplorationState state_;
};

} // namespace meshmerize