#pragma once

#include <cstdint>

namespace meshmerize {

enum class Direction : std::uint8_t {
    North = 0,
    East,
    South,
    West
};

enum class RelativeDirection : std::uint8_t {
    Left = 0,
    Straight,
    Right,
    Back
};

enum class RobotMode : std::uint8_t {
    Idle = 0,
    Calibration,
    DryRun,
    Planning,
    ActualRun,
    Completed,
    Fault
};

enum class Action : std::uint8_t {
    Forward = 0,
    Left,
    Right,
    UTurn,
    Stop
};

} // namespace meshmerize