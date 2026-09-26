#include <cassert>

#include "maze/edge.h"
#include "maze/maze_map.h"
#include "maze/node.h"
#include "perception/line_sensor.h"
#include "simulator/virtual_robot.h"
#include "simulator/virtual_sensor.h"

int main()
{
    using namespace meshmerize;

    MazeMap maze;

    Node start;
    start.id = 0;
    start.x = 0.0;
    start.y = 0.0;
    start.is_start = true;

    assert(maze.graph().addNode(start));
    assert(maze.setStartNode(0));

    VirtualRobot robot(maze);

    VirtualSensor sensor(robot);

    LineSensorReading reading = sensor.read();

    // Correct number of sensors.
    static_assert(
        LINE_SENSOR_COUNT == 8,
        "Robot must use 8 line sensors"
    );

    // Line must be detected.
    assert(reading.line_detected);

    // Center sensors should detect the line.
    assert(reading.values[3] == 1.0);
    assert(reading.values[4] == 1.0);

    // Outer sensors should not detect the centered line.
    assert(reading.values[0] == 0.0);
    assert(reading.values[1] == 0.0);
    assert(reading.values[2] == 0.0);
    assert(reading.values[5] == 0.0);
    assert(reading.values[6] == 0.0);
    assert(reading.values[7] == 0.0);

    return 0;
}