#include <cassert>

#include "core/types.h"
#include "robot/robot_state.h"

int main() {
    using namespace meshmerize;

    RobotState robot;

    assert(robot.node_id == -1);
    assert(robot.direction == Direction::North);
    assert(robot.mode == RobotMode::Idle);

    robot.direction = Direction::East;

    assert(robot.direction == Direction::East);

    robot.mode = RobotMode::DryRun;

    assert(robot.mode == RobotMode::DryRun);

    return 0;
}