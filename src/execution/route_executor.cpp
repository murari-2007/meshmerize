#include "execution/route_executor.h"

namespace meshmerize {

RouteExecutor::RouteExecutor(
    const TurnConfig& turn_config
)
    : turn_controller_(turn_config)
{
}

void RouteExecutor::loadRoute(const Route& route)
{
    route_ = route;

    current_action_index_ = 0;

    route_loaded_ = true;

    state_ = RouteExecutorState::Idle;

    turn_controller_.reset();
}

void RouteExecutor::start()
{
    if (!route_loaded_ || route_.empty()) {
        state_ = RouteExecutorState::Fault;
        return;
    }

    current_action_index_ = 0;

    state_ = RouteExecutorState::Idle;
}

void RouteExecutor::update(
    const RouteExecutorFeedback& feedback
)
{
    if (!route_loaded_) {
        state_ = RouteExecutorState::Fault;
        return;
    }

    if (state_ == RouteExecutorState::Completed ||
        state_ == RouteExecutorState::Fault) {
        return;
    }

    if (current_action_index_ >= route_.size()) {
        state_ = RouteExecutorState::Completed;
        return;
    }

    if (state_ == RouteExecutorState::Idle) {
        startCurrentAction(feedback);
        return;
    }

    if (state_ == RouteExecutorState::Turning) {

        const TurnCommand command =
            turn_controller_.update(
                feedback.turn_feedback
            );

        if (command.complete) {

            ++current_action_index_;

            if (current_action_index_ >= route_.size()) {
                state_ = RouteExecutorState::Completed;
                return;
            }

            startCurrentAction(feedback);
        }

        return;
    }

    if (state_ == RouteExecutorState::Forward) {
        /*
         * Actual forward-motion completion will be
         * connected to line/motion control later.
         *
         * For Phase 18, forward is represented as
         * an active state only.
         */
        return;
    }
}

void RouteExecutor::startCurrentAction(
    const RouteExecutorFeedback& feedback
)
{
    const Action action =
        route_.actions[current_action_index_];

    switch (action) {

        case Action::Forward:
            state_ = RouteExecutorState::Forward;
            break;

        case Action::Left:
            turn_controller_.start(
                RelativeDirection::Left,
                feedback.turn_feedback
            );

            state_ = RouteExecutorState::Turning;
            break;

        case Action::Right:
            turn_controller_.start(
                RelativeDirection::Right,
                feedback.turn_feedback
            );

            state_ = RouteExecutorState::Turning;
            break;

        case Action::UTurn:
            turn_controller_.start(
                RelativeDirection::Back,
                feedback.turn_feedback
            );

            state_ = RouteExecutorState::Turning;
            break;

        case Action::Stop:
            state_ = RouteExecutorState::Completed;
            break;
    }
}

void RouteExecutor::reset()
{
    route_.clear();

    route_loaded_ = false;

    current_action_index_ = 0;

    state_ = RouteExecutorState::Idle;

    turn_controller_.reset();
}

RouteExecutorState RouteExecutor::state() const
{
    return state_;
}

bool RouteExecutor::completed() const
{
    return state_ == RouteExecutorState::Completed;
}

bool RouteExecutor::active() const
{
    return state_ == RouteExecutorState::Forward ||
           state_ == RouteExecutorState::Turning;
}

std::size_t RouteExecutor::currentActionIndex() const
{
    return current_action_index_;
}

Action RouteExecutor::currentAction() const
{
    if (route_.empty() ||
        current_action_index_ >= route_.size()) {
        return Action::Stop;
    }

    return route_.actions[current_action_index_];
}

} // namespace meshmerize