#include "perception/end_detector.h"

namespace meshmerize {

bool EndZoneDetector::isWideWhite(
    const LineSensorReading& reading
) const
{
    for (double value : reading.values) {
        if (value > WHITE_THRESHOLD) {
            return false;
        }
    }

    return true;
}

bool EndZoneDetector::detect(
    const LineSensorReading& reading
)
{
    const bool wide_white = isWideWhite(reading);

    /*
     * The robot must first establish that it has
     * encountered the line.
     *
     * Once a line has been seen, the detector can
     * continuously evaluate white-area confirmation.
     */
    if (reading.line_detected) {
        line_seen_ = true;
    }

    if (!line_seen_) {
        return false;
    }

    if (wide_white) {
        ++confirmation_count_;
    } else {
        confirmation_count_ = 0;
    }

    if (confirmation_count_ >= REQUIRED_CONFIRMATIONS) {
        return true;
    }

    return false;
}

void EndZoneDetector::reset()
{
    confirmation_count_ = 0;
    line_seen_ = false;
}

} // namespace meshmerize