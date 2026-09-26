#include <cassert>

#include "hardware/button_interface.h"
#include "hardware/buzzer_interface.h"
#include "hardware/encoder_interface.h"
#include "hardware/led_interface.h"
#include "hardware/motor_interface.h"
#include "hardware/sensor_interface.h"

using namespace meshmerize;

class FakeSensor : public SensorInterface {
public:
    LineSensorReading reading;

    LineSensorReading readLine() override {
        return reading;
    }
};

class FakeMotor : public MotorInterface {
public:
    double left = 0.0;
    double right = 0.0;
    bool stopped = false;

    void setLeftMotor(double command) override {
        left = command;
        stopped = false;
    }

    void setRightMotor(double command) override {
        right = command;
        stopped = false;
    }

    void stop() override {
        left = 0.0;
        right = 0.0;
        stopped = true;
    }
};

class FakeEncoder : public EncoderInterface {
public:
    EncoderReading reading;

    EncoderReading read() override {
        return reading;
    }

    void reset() override {
        reading.left_ticks = 0.0;
        reading.right_ticks = 0.0;
    }
};

class FakeButton : public ButtonInterface {
public:
    bool pressed = false;

    bool isPressed() override {
        return pressed;
    }
};

class FakeLed : public LedInterface {
public:
    bool enabled = false;

    void set(bool value) override {
        enabled = value;
    }

    void on() override {
        enabled = true;
    }

    void off() override {
        enabled = false;
    }
};

class FakeBuzzer : public BuzzerInterface {
public:
    bool enabled = false;
    unsigned int last_duration = 0;

    void on() override {
        enabled = true;
    }

    void off() override {
        enabled = false;
    }

    void beep(unsigned int duration_ms) override {
        enabled = true;
        last_duration = duration_ms;
        enabled = false;
    }
};

void test_sensor_interface()
{
    FakeSensor sensor;

    sensor.reading.line_detected = true;

    const LineSensorReading reading =
        sensor.readLine();

    assert(reading.line_detected);
}

void test_motor_interface()
{
    FakeMotor motor;

    motor.setLeftMotor(0.5);
    motor.setRightMotor(0.6);

    assert(motor.left == 0.5);
    assert(motor.right == 0.6);
    assert(!motor.stopped);

    motor.stop();

    assert(motor.left == 0.0);
    assert(motor.right == 0.0);
    assert(motor.stopped);
}

void test_encoder_interface()
{
    FakeEncoder encoder;

    encoder.reading.left_ticks = 120.0;
    encoder.reading.right_ticks = 125.0;

    const EncoderReading reading =
        encoder.read();

    assert(reading.left_ticks == 120.0);
    assert(reading.right_ticks == 125.0);

    encoder.reset();

    assert(encoder.reading.left_ticks == 0.0);
    assert(encoder.reading.right_ticks == 0.0);
}

void test_button_interface()
{
    FakeButton button;

    assert(!button.isPressed());

    button.pressed = true;

    assert(button.isPressed());
}

void test_led_interface()
{
    FakeLed led;

    led.on();

    assert(led.enabled);

    led.off();

    assert(!led.enabled);

    led.set(true);

    assert(led.enabled);
}

void test_buzzer_interface()
{
    FakeBuzzer buzzer;

    buzzer.on();

    assert(buzzer.enabled);

    buzzer.off();

    assert(!buzzer.enabled);

    buzzer.beep(100);

    assert(!buzzer.enabled);
    assert(buzzer.last_duration == 100);
}

int main()
{
    test_sensor_interface();
    test_motor_interface();
    test_encoder_interface();
    test_button_interface();
    test_led_interface();
    test_buzzer_interface();

    return 0;
}
