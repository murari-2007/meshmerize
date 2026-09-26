#include "perception/junction_detector.h"

namespace meshmerize {

namespace {

constexpr double DETECTION_THRESHOLD = 0.5;

}

bool JunctionDetector::groupDetected(
    const LineSensorReading& reading,
    std::size_t start,
    std::size_t end
) const
{
    for (std::size_t i = start; i <= end; ++i) {
        if (reading.values[i] >= DETECTION_THRESHOLD) {
            return true;
        }
    }

    return false;
}

JunctionObservation JunctionDetector::detect(
    const LineSensorReading& reading
) const
{
    JunctionObservation result;

    result.left = groupDetected(reading, 0, 2);
    result.straight = groupDetected(reading, 3, 4);
    result.right = groupDetected(reading, 5, 7);

    /*
     * A junction requires more than the normal centered line.
     *
     * A normal straight line usually has only the center
     * sensors active.
     *
     * Therefore at least one side must also be active.
     */
    result.detected =
        (result.left && result.straight) ||
        (result.straight && result.right) ||
        (result.left && result.right);

    return result;
}

} // namespace meshmerize