#include "perception/line_position.h"

namespace meshmerize {

LinePosition LinePositionCalculator::calculate(
    const LineSensorReading& reading
) const
{
    LinePosition result;

    constexpr double sensor_positions[LINE_SENSOR_COUNT] = {
        -3.5,
        -2.5,
        -1.5,
        -0.5,
        0.5,
        1.5,
        2.5,
        3.5
    };

    double weighted_sum = 0.0;
    double total_weight = 0.0;

    for (std::size_t i = 0; i < LINE_SENSOR_COUNT; ++i) {
        const double value = reading.values[i];

        weighted_sum +=
            sensor_positions[i] * value;

        total_weight += value;
    }

    // No sensor sees the line.
    if (total_weight <= 0.0) {
        result.position = 0.0;
        result.line_detected = false;

        return result;
    }

    result.position =
        weighted_sum / total_weight;

    result.line_detected = true;

    return result;
}

} // namespace meshmerize