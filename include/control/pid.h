#pragma once

#include "core/config.h"

namespace meshmerize {

class PIDController {
public:
    explicit PIDController(const PIDConfig& config);

    double update(double error, double dt);

    void reset();

private:
    PIDConfig config_;

    double integral_ = 0.0;
    double previous_error_ = 0.0;
    bool initialized_ = false;
};

} // namespace meshmerize