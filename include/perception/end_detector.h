#pragma once

#include "perception/line_sensor.h"

namespace meshmerize {

class EndZoneDetector {
public:
    bool detect(
        const LineSensorReading& reading
    );

    void reset();

private:
    static constexpr double WHITE_THRESHOLD = 0.2;
    static constexpr int REQUIRED_CONFIRMATIONS = 3;

    int confirmation_count_ = 0;
    bool line_seen_ = false;
    bool isWideWhite(
        const LineSensorReading& reading
    ) const;
};

} // namespace meshmerize