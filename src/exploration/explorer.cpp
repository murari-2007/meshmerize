#include "exploration/explorer.h"
#include "robot/orientation.h"

namespace meshmerize {

void Explorer::reset()
{
    state_.reset();
}

JunctionDecision Explorer::chooseDirection(
    const JunctionObservation& observation,
    bool straight_explored,
    bool left_explored,
    bool right_explored
) const
{
    return chooseSLRB(
        observation,
        straight_explored,
        left_explored,
        right_explored
    );
}

bool Explorer::enterBranch(
    int current_node,
    RelativeDirection direction
)
{
    Direction exit_heading = applyRelativeTurn(
        state_.heading(),
        direction
    );

    if (!state_.push(
            current_node,
            exit_heading
        )) {
        return false;
    }

    state_.updateHeading(direction);

    return true;
}

bool Explorer::backtrack(
    ExplorationFrame& frame
)
{
    if (!state_.pop(frame)) {
        return false;
    }

    /*
     * We have physically returned to the stored node.
     *
     * The robot's heading has already been updated by the
     * physical U-turn/backtracking action in the real robot.
     *
     * Phase 12 only maintains the logical exploration state.
     */

    return true;
}

void Explorer::updateHeading(
    RelativeDirection direction
)
{
    state_.updateHeading(direction);
}

Direction Explorer::heading() const
{
    return state_.heading();
}

std::size_t Explorer::depth() const
{
    return state_.depth();
}

} // namespace meshmerize