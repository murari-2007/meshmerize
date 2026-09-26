#pragma once

#include "core/types.h"
#include "perception/line_sensor.h"

namespace meshmerize {

class SensorInterface {
public:
    virtual ~SensorInterface() = default;

    virtual LineSensorReading readLine() = 0;
};

} // namespace meshmerize
