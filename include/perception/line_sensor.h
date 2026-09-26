#pragma once

#include <array>
#include <cstddef>

namespace meshmerize {

constexpr std::size_t LINE_SENSOR_COUNT = 8;

struct LineSensorReading {
    std::array<double, LINE_SENSOR_COUNT> values{};

    bool line_detected = false;
};

class LineSensor {
public:
    virtual ~LineSensor() = default;

    virtual LineSensorReading read() = 0;
};

} // namespace meshmerize