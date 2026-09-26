#include "simulator/virtual_hardware.h"

namespace meshmerize {

VirtualSensorAdapter::VirtualSensorAdapter(
    const VirtualRobot& robot
)
    : sensor_(robot)
{
}

LineSensorReading VirtualSensorAdapter::readLine()
{
    return sensor_.read();
}

void VirtualMotor::setLeftMotor(double command)
{
    left_command_ = command;
}

void VirtualMotor::setRightMotor(double command)
{
    right_command_ = command;
}

void VirtualMotor::stop()
{
    left_command_ = 0.0;
    right_command_ = 0.0;
}

double VirtualMotor::leftCommand() const
{
    return left_command_;
}

double VirtualMotor::rightCommand() const
{
    return right_command_;
}

VirtualEncoder::VirtualEncoder(
    const VirtualRobot& robot
)
    : robot_(robot)
{
}

EncoderReading VirtualEncoder::read()
{
    EncoderReading reading;

    reading.left_ticks =
        robot_.state().left_encoder - left_offset_;

    reading.right_ticks =
        robot_.state().right_encoder - right_offset_;

    return reading;
}

void VirtualEncoder::reset()
{
    left_offset_ =
        robot_.state().left_encoder;

    right_offset_ =
        robot_.state().right_encoder;
}

bool VirtualButton::isPressed()
{
    return pressed_;
}

void VirtualButton::setPressed(bool pressed)
{
    pressed_ = pressed;
}

void VirtualLed::set(bool enabled)
{
    enabled_ = enabled;
}

void VirtualLed::on()
{
    enabled_ = true;
}

void VirtualLed::off()
{
    enabled_ = false;
}

bool VirtualLed::enabled() const
{
    return enabled_;
}

void VirtualBuzzer::on()
{
    enabled_ = true;
}

void VirtualBuzzer::off()
{
    enabled_ = false;
}

void VirtualBuzzer::beep(unsigned int duration_ms)
{
    last_duration_ms_ = duration_ms;

    /*
     * The simulator records the beep event.
     * No real-time delay is introduced.
     */
    enabled_ = false;
}

bool VirtualBuzzer::enabled() const
{
    return enabled_;
}

unsigned int VirtualBuzzer::lastDurationMs() const
{
    return last_duration_ms_;
}

} // namespace meshmerize
