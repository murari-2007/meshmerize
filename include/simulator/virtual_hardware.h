#pragma once

#include "hardware/button_interface.h"
#include "hardware/buzzer_interface.h"
#include "hardware/encoder_interface.h"
#include "hardware/led_interface.h"
#include "hardware/motor_interface.h"
#include "hardware/sensor_interface.h"
#include "simulator/virtual_robot.h"
#include "simulator/virtual_sensor.h"

namespace meshmerize {

class VirtualSensorAdapter : public SensorInterface {
public:
    explicit VirtualSensorAdapter(const VirtualRobot& robot);

    LineSensorReading readLine() override;

private:
    VirtualSensor sensor_;
};

class VirtualMotor : public MotorInterface {
public:
    void setLeftMotor(double command) override;

    void setRightMotor(double command) override;

    void stop() override;

    double leftCommand() const;

    double rightCommand() const;

private:
    double left_command_ = 0.0;
    double right_command_ = 0.0;
};

class VirtualEncoder : public EncoderInterface {
public:
    explicit VirtualEncoder(const VirtualRobot& robot);

    EncoderReading read() override;

    void reset() override;

private:
    const VirtualRobot& robot_;

    double left_offset_ = 0.0;
    double right_offset_ = 0.0;
};

class VirtualButton : public ButtonInterface {
public:
    bool isPressed() override;

    void setPressed(bool pressed);

private:
    bool pressed_ = false;
};

class VirtualLed : public LedInterface {
public:
    void set(bool enabled) override;

    void on() override;

    void off() override;

    bool enabled() const;

private:
    bool enabled_ = false;
};

class VirtualBuzzer : public BuzzerInterface {
public:
    void on() override;

    void off() override;

    void beep(unsigned int duration_ms) override;

    bool enabled() const;

    unsigned int lastDurationMs() const;

private:
    bool enabled_ = false;
    unsigned int last_duration_ms_ = 0;
};

} // namespace meshmerize
