#include "planning/path_optimizer.h"

namespace meshmerize {

Route PathOptimizer::optimize(const Route& route) const
{
    Route optimized;

    for (const Action action : route.actions) {
        if (action == Action::Stop) {
            continue;
        }

        optimized.actions.push_back(action);
    }

    optimized.actions.push_back(Action::Stop);

    return optimized;
}

} // namespace meshmerize