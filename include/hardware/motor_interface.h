#pragma once

namespace meshmerize {

class MotorInterface {
public:
    virtual ~MotorInterface() = default;

    /*
     * Motor command range:
     *
     * -1.0 = full reverse
     *  0.0 = stop
     * +1.0 = full forward
     */
    virtual void setLeftMotor(double command) = 0;

    virtual void setRightMotor(double command) = 0;

    virtual void stop() = 0;
};

} // namespace meshmerize
