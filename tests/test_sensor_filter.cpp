#include <cassert>
#include <cmath>

#include "perception/line_sensor.h"
#include "perception/sensor_filter.h"

int main()
{
    using namespace meshmerize;

    SensorFilter filter;

    LineSensorReading first;

    first.values = {
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0
    };

    first.line_detected = true;

    // First reading passes through unchanged.
    LineSensorReading result1 = filter.update(first);

    assert(result1.values[3] == 1.0);
    assert(result1.values[4] == 1.0);
    assert(result1.line_detected);

    LineSensorReading second;

    second.values = {
        0.0,
        0.2,
        0.4,
        0.8,
        0.6,
        0.2,
        0.0,
        0.0
    };

    LineSensorReading result2 = filter.update(second);

    // Moving average.
    assert(std::abs(result2.values[0] - 0.0) < 1e-9);
    assert(std::abs(result2.values[1] - 0.1) < 1e-9);
    assert(std::abs(result2.values[2] - 0.2) < 1e-9);
    assert(std::abs(result2.values[3] - 0.9) < 1e-9);
    assert(std::abs(result2.values[4] - 0.8) < 1e-9);
    assert(std::abs(result2.values[5] - 0.1) < 1e-9);

    assert(result2.line_detected);

    // Reset should remove filter history.
    filter.reset();

    LineSensorReading after_reset = filter.update(second);

    // First reading after reset passes through unchanged.
    assert(after_reset.values[1] == 0.2);
    assert(after_reset.values[3] == 0.8);
    assert(after_reset.values[4] == 0.6);

    return 0;
}