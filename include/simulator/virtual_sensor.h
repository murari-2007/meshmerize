#pragma once
#include "core/types.h"
#include "perception/line_sensor.h"
#include "simulator/virtual_robot.h"

namespace meshmerize {

class VirtualSensor : public LineSensor {
public:
    explicit VirtualSensor(const VirtualRobot& robot);

    LineSensorReading read() override;

private:
    const VirtualRobot& robot_;

    LineSensorReading createStraightLineReading() const;

    LineSensorReading createJunctionReading(
        bool left,
        bool straight,
        bool right
    ) const;

    LineSensorReading createEndZoneReading() const;

    Direction relativeToAbsolute(
        RelativeDirection direction
    ) const;
};

} // namespace meshmerize