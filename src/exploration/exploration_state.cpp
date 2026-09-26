#include "exploration/exploration_state.h"

#include "robot/orientation.h"

namespace meshmerize {

void ExplorationState::reset()
{
    stack_size_ = 0;
    heading_ = Direction::North;
}

bool ExplorationState::push(
    int node_id,
    Direction exit_heading
)
{
    if (stack_size_ >= MAX_DEPTH) {
        return false;
    }

    stack_[stack_size_].node_id = node_id;
    stack_[stack_size_].exit_heading = exit_heading;

    ++stack_size_;

    return true;
}

bool ExplorationState::pop(
    ExplorationFrame& frame
)
{
    if (stack_size_ == 0) {
        return false;
    }

    --stack_size_;

    frame = stack_[stack_size_];

    return true;
}

bool ExplorationState::empty() const
{
    return stack_size_ == 0;
}

std::size_t ExplorationState::depth() const
{
    return stack_size_;
}

Direction ExplorationState::heading() const
{
    return heading_;
}

void ExplorationState::setHeading(Direction heading)
{
    heading_ = heading;
}

void ExplorationState::updateHeading(
    RelativeDirection turn
)
{
    heading_ = applyRelativeTurn(
        heading_,
        turn
    );
}

} // namespace meshmerize