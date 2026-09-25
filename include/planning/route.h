#pragma once

#include <vector>

#include "core/types.h"

namespace meshmerize {

struct Route {
    std::vector<Action> actions;

    bool empty() const {
        return actions.empty();
    }

    std::size_t size() const {
        return actions.size();
    }

    void clear() {
        actions.clear();
    }
};

} // namespace meshmerize