#pragma once

#include "core/types.h"

namespace meshmerize {

struct RobotState {
    int node_id = -1;

    Direction direction = Direction::North;
    RobotMode mode = RobotMode::Idle;

    double x = 0.0;
    double y = 0.0;

    double left_encoder = 0.0;
    double right_encoder = 0.0;

    double velocity = 0.0;

    bool line_detected = false;
    bool junction_detected = false;
    bool end_detected = false;
};

} // namespace meshmerize