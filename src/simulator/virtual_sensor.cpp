#include "simulator/virtual_sensor.h"

namespace meshmerize {

namespace {

void setGroup(
    LineSensorReading& reading,
    std::size_t start,
    std::size_t end
)
{
    for (std::size_t i = start; i <= end; ++i) {
        reading.values[i] = 1.0;
    }
}

}

VirtualSensor::VirtualSensor(const VirtualRobot& robot)
    : robot_(robot)
{
}

LineSensorReading VirtualSensor::read()
{
    const RobotState& state = robot_.state();

    /*
     * The end node is represented by a wide white area.
     */
    if (state.node_id >= 0 &&
        robot_.hasConnectionInDirection(state.direction) == false) {
        /*
         * Do not classify every dead end as an end zone.
         *
         * Phase 22 only models the physical end zone through
         * the maze's end node in a later refinement.
         *
         * For now, return the normal line.
         */
    }

    const bool left =
        robot_.hasConnectionInDirection(
            relativeToAbsolute(RelativeDirection::Left)
        );

    const bool straight =
        robot_.hasConnectionInDirection(
            relativeToAbsolute(RelativeDirection::Straight)
        );

    const bool right =
        robot_.hasConnectionInDirection(
            relativeToAbsolute(RelativeDirection::Right)
        );

    if (!left && !right) {
        return createStraightLineReading();
    }

    return createJunctionReading(
        left,
        straight,
        right
    );
}

LineSensorReading VirtualSensor::createStraightLineReading() const
{
    LineSensorReading reading;

    reading.values = {
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0
    };

    reading.line_detected = true;

    return reading;
}

LineSensorReading VirtualSensor::createJunctionReading(
    bool left,
    bool straight,
    bool right
) const
{
    LineSensorReading reading;

    reading.values = {
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    if (left) {
        setGroup(reading, 0, 2);
    }

    if (straight) {
        setGroup(reading, 3, 4);
    }

    if (right) {
        setGroup(reading, 5, 7);
    }

    reading.line_detected =
        left || straight || right;

    return reading;
}

LineSensorReading VirtualSensor::createEndZoneReading() const
{
    LineSensorReading reading;

    reading.values = {
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    reading.line_detected = false;

    return reading;
}

Direction VirtualSensor::relativeToAbsolute(
    RelativeDirection direction
) const
{
    const Direction heading =
        robot_.state().direction;

    switch (direction) {

    case RelativeDirection::Left:
        switch (heading) {
        case Direction::North:
            return Direction::West;
        case Direction::East:
            return Direction::North;
        case Direction::South:
            return Direction::East;
        case Direction::West:
            return Direction::South;
        }
        break;

    case RelativeDirection::Straight:
        return heading;

    case RelativeDirection::Right:
        switch (heading) {
        case Direction::North:
            return Direction::East;
        case Direction::East:
            return Direction::South;
        case Direction::South:
            return Direction::West;
        case Direction::West:
            return Direction::North;
        }
        break;

    case RelativeDirection::Back:
        switch (heading) {
        case Direction::North:
            return Direction::South;
        case Direction::East:
            return Direction::West;
        case Direction::South:
            return Direction::North;
        case Direction::West:
            return Direction::East;
        }
        break;
    }

    return Direction::North;
}

} // namespace meshmerize