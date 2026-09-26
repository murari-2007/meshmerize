#pragma once

#include <vector>

#include "core/types.h"

namespace meshmerize {

class ExplorationTrace {
public:
    void reset();

    void record(Action action);

    const std::vector<Action>& actions() const;

    std::size_t size() const;

private:
    std::vector<Action> actions_;
};

} // namespace meshmerize
