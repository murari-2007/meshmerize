#pragma once

namespace meshmerize {

struct PIDConfig {
    double kp = 0.0;
    double ki = 0.0;
    double kd = 0.0;

    double output_min = -255.0;
    double output_max = 255.0;
};

struct RobotConfig {
    double base_speed_dry_run = 0.0;
    double base_speed_actual = 0.0;

    double turn_speed = 0.0;

    double max_motor_command = 255.0;

    double sensor_threshold = 0.5;
};

} // namespace meshmerize