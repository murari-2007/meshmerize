#include <cassert>

#include "maze/maze_builder.h"
#include "maze/maze_map.h"
#include "perception/end_detector.h"
#include "simulator/virtual_robot.h"
#include "simulator/virtual_sensor.h"

using namespace meshmerize;

static MazeMap buildEndMaze()
{
    MazeMap maze;
    MazeBuilder builder(maze);

    assert(builder.addStartNode(
        0,
        0.0,
        0.0
    ));

    assert(builder.addDiscoveredNode(
        1,
        0.0,
        100.0
    ));

    assert(builder.addDiscoveredNode(
        2,
        0.0,
        200.0
    ));

    assert(builder.connectNodes(
        0,
        1,
        Direction::North,
        100.0
    ));

    assert(builder.connectNodes(
        1,
        2,
        Direction::North,
        100.0
    ));

    assert(maze.setEndNode(2));

    return maze;
}

static void test_end_node_is_detected()
{
    MazeMap maze = buildEndMaze();

   VirtualRobot robot(maze);
VirtualSensor sensor(robot);
EndZoneDetector detector;

assert(!robot.isAtEndNode());

/*
 * Establish that the detector has previously
 * seen the line.
 */
assert(!detector.detect(sensor.read()));

assert(robot.moveForward(100.0));
assert(robot.state().node_id == 1);
assert(!robot.isAtEndNode());

assert(!detector.detect(sensor.read()));

assert(robot.moveForward(100.0));
assert(robot.state().node_id == 2);
assert(robot.isAtEndNode());

/*
 * EndZoneDetector intentionally requires
 * three consecutive confirmations.
 */
assert(!detector.detect(sensor.read()));
assert(!detector.detect(sensor.read()));
assert(detector.detect(sensor.read()));
}

static void test_dead_end_is_not_end_zone()
{
    MazeMap maze;
    MazeBuilder builder(maze);

    assert(builder.addStartNode(
        0,
        0.0,
        0.0
    ));

    assert(builder.addDiscoveredNode(
        1,
        0.0,
        100.0
    ));

    assert(builder.connectNodes(
        0,
        1,
        Direction::North,
        100.0
    ));

    /*
     * Deliberately do NOT call setEndNode().
     */
    assert(maze.endNode() == -1);

    VirtualRobot robot(maze);
    VirtualSensor sensor(robot);
    EndZoneDetector detector;

    assert(robot.moveForward(100.0));
    assert(robot.state().node_id == 1);
    assert(!robot.isAtEndNode());

    /*
     * A dead end remains a normal line in the simulator.
     */
    const LineSensorReading reading = sensor.read();

    assert(reading.line_detected);

    assert(!detector.detect(reading));
}

int main()
{
    test_end_node_is_detected();
    test_dead_end_is_not_end_zone();

    return 0;
}
