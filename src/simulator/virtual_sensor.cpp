#include "simulator/virtual_sensor.h"

namespace meshmerize {

VirtualSensor::VirtualSensor(const VirtualRobot& robot)
    : robot_(robot)
{
}

LineSensorReading VirtualSensor::read()
{
    return createStraightLineReading();
}

LineSensorReading VirtualSensor::createStraightLineReading() const
{
    LineSensorReading reading;

    /*
     * Phase 5 intentionally uses a simple centered line model.
     *
     * The robot is assumed to be centered on a straight line.
     * Later phases will introduce:
     *
     * - lateral offset
     * - curves
     * - junctions
     * - dead ends
     * - end zones
     */

    reading.values = {
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0
    };

    reading.line_detected = true;

    return reading;
}

} // namespace meshmerize