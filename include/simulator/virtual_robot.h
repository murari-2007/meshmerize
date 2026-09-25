#pragma once

#include "maze/maze_map.h"
#include "robot/robot_state.h"

namespace meshmerize {

class VirtualRobot {
public:
    explicit VirtualRobot(MazeMap& maze);

    const RobotState& state() const;

    RobotState& state();

    bool moveForward(double distance);

    bool turnLeft();

    bool turnRight();

    bool turnAround();

private:
    MazeMap& maze_;
    RobotState state_;

    bool canMoveTo(int target_node_id) const;

    int findNeighborInDirection(Direction direction) const;
};

} // namespace meshmerize