#pragma once

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
};

} // namespace meshmerize