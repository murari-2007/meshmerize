#include <cassert>

#include "simulator/virtual_hardware.h"

using namespace meshmerize;

void test_virtual_sensor()
{
    MazeMap maze;

    maze.graph().addNode({0, 0.0, 0.0, true, false});
    maze.graph().addNode({1, 0.0, 100.0, false, true});

    maze.graph().addEdge({
        0,
        1,
        100.0,
        Direction::North,
        false
    });

    maze.setStartNode(0);
    maze.setEndNode(1);

    VirtualRobot robot(maze);

    VirtualSensorAdapter sensor(robot);

    const LineSensorReading reading =
        sensor.readLine();

    assert(reading.line_detected);
}

void test_virtual_motor()
{
    VirtualMotor motor;

    motor.setLeftMotor(0.5);
    motor.setRightMotor(0.7);

    assert(motor.leftCommand() == 0.5);
    assert(motor.rightCommand() == 0.7);

    motor.stop();

    assert(motor.leftCommand() == 0.0);
    assert(motor.rightCommand() == 0.0);
}

void test_virtual_encoder()
{
    MazeMap maze;

    maze.graph().addNode({0, 0.0, 0.0, true, false});
    maze.graph().addNode({1, 0.0, 100.0, false, true});

    maze.graph().addEdge({
        0,
        1,
        100.0,
        Direction::North,
        false
    });

    maze.setStartNode(0);
    maze.setEndNode(1);

    VirtualRobot robot(maze);

    VirtualEncoder encoder(robot);

    EncoderReading initial =
        encoder.read();

    assert(initial.left_ticks == 0.0);
    assert(initial.right_ticks == 0.0);

    assert(robot.moveForward(100.0));

    EncoderReading moved =
        encoder.read();

    assert(moved.left_ticks == 100.0);
    assert(moved.right_ticks == 100.0);

    encoder.reset();

    EncoderReading reset =
        encoder.read();

    assert(reset.left_ticks == 0.0);
    assert(reset.right_ticks == 0.0);
}

void test_virtual_button()
{
    VirtualButton button;

    assert(!button.isPressed());

    button.setPressed(true);

    assert(button.isPressed());

    button.setPressed(false);

    assert(!button.isPressed());
}

void test_virtual_led()
{
    VirtualLed led;

    assert(!led.enabled());

    led.on();

    assert(led.enabled());

    led.off();

    assert(!led.enabled());

    led.set(true);

    assert(led.enabled());
}

void test_virtual_buzzer()
{
    VirtualBuzzer buzzer;

    assert(!buzzer.enabled());

    buzzer.on();

    assert(buzzer.enabled());

    buzzer.off();

    assert(!buzzer.enabled());

    buzzer.beep(100);

    assert(buzzer.lastDurationMs() == 100);
    assert(!buzzer.enabled());
}

int main()
{
    test_virtual_sensor();
    test_virtual_motor();
    test_virtual_encoder();
    test_virtual_button();
    test_virtual_led();
    test_virtual_buzzer();

    return 0;
}
