#include "exploration/exploration_trace.h"

namespace meshmerize {

void ExplorationTrace::reset()
{
    actions_.clear();
}

void ExplorationTrace::record(Action action)
{
    actions_.push_back(action);
}

const std::vector<Action>& ExplorationTrace::actions() const
{
    return actions_;
}

std::size_t ExplorationTrace::size() const
{
    return actions_.size();
}

} // namespace meshmerize
