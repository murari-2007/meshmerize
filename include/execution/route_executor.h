#pragma once

#include "control/turn_controller.h"
#include "planning/route.h"

namespace meshmerize {

enum class RouteExecutorState {
    Idle,
    Forward,
    Turning,
    Completed,
    Fault
};

struct RouteExecutorFeedback {
    TurnFeedback turn_feedback;

    /*
     * Forward motion is completed by the
     * motion/line controller.
     *
     * The RouteExecutor only consumes the
     * completion signal.
     */
    bool forward_complete = false;
};

class RouteExecutor {
public:
    explicit RouteExecutor(const TurnConfig& turn_config);

    void loadRoute(const Route& route);

    void start();

    void update(const RouteExecutorFeedback& feedback);

    void reset();

    RouteExecutorState state() const;

    bool completed() const;

    bool active() const;

    std::size_t currentActionIndex() const;

    Action currentAction() const;

private:
    void startCurrentAction(
        const RouteExecutorFeedback& feedback
    );

    Route route_;

    TurnController turn_controller_;

    RouteExecutorState state_ =
        RouteExecutorState::Idle;

    std::size_t current_action_index_ = 0;

    bool route_loaded_ = false;
};

} // namespace meshmerize