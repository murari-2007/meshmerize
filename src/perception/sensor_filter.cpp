#include "perception/sensor_filter.h"

namespace meshmerize {

SensorFilter::SensorFilter()
{
    reset();
}

LineSensorReading SensorFilter::update(
    const LineSensorReading& reading
)
{
    if (!initialized_) {
        previous_reading_ = reading;
        initialized_ = true;

        return reading;
    }

    LineSensorReading filtered;

    for (std::size_t i = 0; i < LINE_SENSOR_COUNT; ++i) {
        filtered.values[i] =
            (previous_reading_.values[i] +
             reading.values[i]) / 2.0;
    }

    /*
     * line_detected is derived from the filtered sensor values.
     */
    filtered.line_detected = false;

    for (double value : filtered.values) {
        if (value > 0.0) {
            filtered.line_detected = true;
            break;
        }
    }

    previous_reading_ = reading;

    return filtered;
}

void SensorFilter::reset()
{
    previous_reading_ = LineSensorReading{};
    initialized_ = false;
}

} // namespace meshmerize