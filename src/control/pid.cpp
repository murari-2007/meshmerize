#include "control/pid.h"

namespace meshmerize {

PIDController::PIDController(const PIDConfig& config)
    : config_(config)
{
}

double PIDController::update(double error, double dt)
{
    // First update: no previous error exists yet.
    if (!initialized_) {
        previous_error_ = error;
        initialized_ = true;
    }

    // Integral term.
    if (dt > 0.0) {
        integral_ += error * dt;
    }

    // Derivative term.
    double derivative = 0.0;

    if (dt > 0.0) {
        derivative = (error - previous_error_) / dt;
    }

    double output =
        config_.kp * error +
        config_.ki * integral_ +
        config_.kd * derivative;

    // Clamp output.
    if (output > config_.output_max) {
        output = config_.output_max;
    }

    if (output < config_.output_min) {
        output = config_.output_min;
    }

    previous_error_ = error;

    return output;
}

void PIDController::reset()
{
    integral_ = 0.0;
    previous_error_ = 0.0;
    initialized_ = false;
}

} // namespace meshmerize