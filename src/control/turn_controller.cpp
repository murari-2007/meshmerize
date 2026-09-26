#include "control/turn_controller.h"

#include <cmath>

namespace meshmerize {

TurnController::TurnController(const TurnConfig& config)
    : config_(config)
{
}

void TurnController::start(
    RelativeDirection direction,
    const TurnFeedback& feedback)
{
    direction_ = direction;

    start_left_encoder_ = feedback.left_encoder;
    start_right_encoder_ = feedback.right_encoder;

    active_ = true;
}

TurnCommand TurnController::update(
    const TurnFeedback& feedback)
{
    TurnCommand command;

    if (!active_) {
        command.complete = true;
        return command;
    }

    if (direction_ == RelativeDirection::Straight) {
        active_ = false;
        command.complete = true;
        return command;
    }

    const double left_delta =
        std::abs(feedback.left_encoder - start_left_encoder_);

    const double right_delta =
        std::abs(feedback.right_encoder - start_right_encoder_);

    const double average_delta =
        (left_delta + right_delta) / 2.0;

    double target = config_.target_ticks_90;

    if (direction_ == RelativeDirection::Back) {
        target *= 2.0;
    }

    if (average_delta >=
        target - config_.tolerance_ticks) {

        active_ = false;

        command.left_motor = 0.0;
        command.right_motor = 0.0;
        command.complete = true;

        return command;
    }

    if (direction_ == RelativeDirection::Left) {
        command.left_motor = -config_.turn_speed;
        command.right_motor = config_.turn_speed;
    }
    else if (direction_ == RelativeDirection::Right) {
        command.left_motor = config_.turn_speed;
        command.right_motor = -config_.turn_speed;
    }
    else if (direction_ == RelativeDirection::Back) {
        command.left_motor = config_.turn_speed;
        command.right_motor = -config_.turn_speed;
    }

    return command;
}

void TurnController::reset()
{
    active_ = false;

    start_left_encoder_ = 0.0;
    start_right_encoder_ = 0.0;
}

bool TurnController::active() const
{
    return active_;
}

} // namespace meshmerize