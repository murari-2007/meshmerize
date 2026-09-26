#pragma once

#include "perception/line_sensor.h"

namespace meshmerize {

struct LinePosition {
    double position = 0.0;
    bool line_detected = false;
};

class LinePositionCalculator {
public:
    LinePosition calculate(
        const LineSensorReading& reading
    ) const;
};

} // namespace meshmerize