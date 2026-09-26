#pragma once

#include "perception/line_sensor.h"

namespace meshmerize {

class SensorFilter {
public:
    SensorFilter();

    LineSensorReading update(
        const LineSensorReading& reading
    );

    void reset();

private:
    LineSensorReading previous_reading_;

    bool initialized_ = false;
};

} // namespace meshmerize