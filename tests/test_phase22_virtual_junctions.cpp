#include <cassert>

#include "maze/maze_builder.h"
#include "maze/maze_map.h"
#include "perception/junction_detector.h"
#include "simulator/virtual_robot.h"
#include "simulator/virtual_sensor.h"

using namespace meshmerize;

static MazeMap buildJunctionMaze()
{
    MazeMap maze;
    MazeBuilder builder(maze);

    /*
     *             2
     *             |
     *             1
     *             |
     *       3 --- 0 --- 4
     *
     * Start = 0
     *
     * From node 0 facing North:
     * Left     -> 3
     * Straight -> 1
     * Right    -> 4
     */

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

    assert(builder.addDiscoveredNode(
        3,
        -100.0,
        0.0
    ));

    assert(builder.addDiscoveredNode(
        4,
        100.0,
        0.0
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

    assert(builder.connectNodes(
        0,
        3,
        Direction::West,
        100.0
    ));

    assert(builder.connectNodes(
        0,
        4,
        Direction::East,
        100.0
    ));

    return maze;
}

static void test_all_three_directions()
{
    MazeMap maze = buildJunctionMaze();

    VirtualRobot robot(maze);
    VirtualSensor sensor(robot);
    JunctionDetector detector;

    const LineSensorReading reading =
        sensor.read();

    const JunctionObservation observation =
        detector.detect(reading);

    assert(observation.detected);

    assert(observation.left);
    assert(observation.straight);
    assert(observation.right);
}

static void test_left_and_straight()
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
        -100.0,
        0.0
    ));

    assert(builder.connectNodes(
        0,
        1,
        Direction::North,
        100.0
    ));

    assert(builder.connectNodes(
        0,
        2,
        Direction::West,
        100.0
    ));

    VirtualRobot robot(maze);
    VirtualSensor sensor(robot);
    JunctionDetector detector;

    const JunctionObservation observation =
        detector.detect(sensor.read());

    assert(observation.detected);

    assert(observation.left);
    assert(observation.straight);
    assert(!observation.right);
}

static void test_right_and_straight()
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
        100.0,
        0.0
    ));

    assert(builder.connectNodes(
        0,
        1,
        Direction::North,
        100.0
    ));

    assert(builder.connectNodes(
        0,
        2,
        Direction::East,
        100.0
    ));

    VirtualRobot robot(maze);
    VirtualSensor sensor(robot);
    JunctionDetector detector;

    const JunctionObservation observation =
        detector.detect(sensor.read());

    assert(observation.detected);

    assert(!observation.left);
    assert(observation.straight);
    assert(observation.right);
}

static void test_normal_straight_line()
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

    VirtualRobot robot(maze);
    VirtualSensor sensor(robot);
    JunctionDetector detector;

    const JunctionObservation observation =
        detector.detect(sensor.read());

    assert(!observation.detected);
    assert(!observation.left);
    assert(observation.straight);
    assert(!observation.right);
}

int main()
{
    test_all_three_directions();
    test_left_and_straight();
    test_right_and_straight();
    test_normal_straight_line();

    return 0;
}
