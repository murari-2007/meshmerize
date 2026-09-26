#pragma once

#include "perception/line_sensor.h"

namespace meshmerize {

struct JunctionObservation {
    bool detected = false;

    bool left = false;
    bool straight = false;
    bool right = false;
};

class JunctionDetector {
public:
    JunctionObservation detect(
        const LineSensorReading& reading
    ) const;

private:
    bool groupDetected(
        const LineSensorReading& reading,
        std::size_t start,
        std::size_t end
    ) const;
};

} // namespace meshmerize