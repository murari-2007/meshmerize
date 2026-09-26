#pragma once

#include "core/types.h"

namespace meshmerize {

struct TurnConfig {
    double target_ticks_90 = 100.0;
    double tolerance_ticks = 3.0;
    double turn_speed = 0.5;
};

struct TurnFeedback {
    double left_encoder = 0.0;
    double right_encoder = 0.0;
};

struct TurnCommand {
    double left_motor = 0.0;
    double right_motor = 0.0;
    bool complete = false;
};

class TurnController {
public:
    explicit TurnController(const TurnConfig& config);

    void start(RelativeDirection direction,
               const TurnFeedback& feedback);

    TurnCommand update(const TurnFeedback& feedback);

    void reset();

    bool active() const;

private:
    TurnConfig config_;

    RelativeDirection direction_ = RelativeDirection::Straight;

    double start_left_encoder_ = 0.0;
    double start_right_encoder_ = 0.0;

    bool active_ = false;
};

} // namespace meshmerize