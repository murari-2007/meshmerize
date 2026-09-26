#include "simulator/virtual_robot.h"

namespace meshmerize {

VirtualRobot::VirtualRobot(MazeMap& maze)
    : maze_(maze)
{
    state_.node_id = maze_.startNode();

    const Node* start_node = maze_.graph().getNode(state_.node_id);

    if (start_node != nullptr) {
        state_.x = start_node->x;
        state_.y = start_node->y;
    }

    state_.direction = Direction::North;
}

const RobotState& VirtualRobot::state() const
{
    return state_;
}

RobotState& VirtualRobot::state()
{
    return state_;
}

bool VirtualRobot::canMoveTo(int target_node_id) const
{
    if (state_.node_id == -1) {
        return false;
    }

    const auto neighbors =
        maze_.graph().getNeighbors(state_.node_id);

    for (int neighbor : neighbors) {
        if (neighbor == target_node_id) {
            return true;
        }
    }

    return false;
}

int VirtualRobot::findNeighborInDirection(Direction direction) const
{
    if (state_.node_id == -1) {
        return -1;
    }

    const auto& edges = maze_.graph().edges();

    for (const Edge& edge : edges) {
        int target = -1;
        Direction edge_direction = edge.direction;

        if (edge.from == state_.node_id) {
            target = edge.to;
        }
        else if (edge.to == state_.node_id) {
            target = edge.from;

            // Reverse direction when travelling from edge.to
            switch (edge.direction) {
            case Direction::North:
                edge_direction = Direction::South;
                break;

            case Direction::East:
                edge_direction = Direction::West;
                break;

            case Direction::South:
                edge_direction = Direction::North;
                break;

            case Direction::West:
                edge_direction = Direction::East;
                break;
            }
        }

        if (target != -1 && edge_direction == direction) {
            return target;
        }
    }

    return -1;
}

bool VirtualRobot::hasConnectionInDirection(
    Direction direction
) const
{
    return findNeighborInDirection(direction) != -1;
}

bool VirtualRobot::moveForward(double distance)
{
    if (distance <= 0.0) {
        return false;
    }

    const int target_node =
        findNeighborInDirection(state_.direction);

    if (target_node == -1) {
        return false;
    }

    if (!canMoveTo(target_node)) {
        return false;
    }

    const Node* target =
        maze_.graph().getNode(target_node);

    if (target == nullptr) {
        return false;
    }

    state_.x = target->x;
    state_.y = target->y;

    state_.node_id = target_node;

    state_.left_encoder += distance;
    state_.right_encoder += distance;

    state_.velocity = distance;

    return true;
}

bool VirtualRobot::turnLeft()
{
    switch (state_.direction) {
    case Direction::North:
        state_.direction = Direction::West;
        break;

    case Direction::West:
        state_.direction = Direction::South;
        break;

    case Direction::South:
        state_.direction = Direction::East;
        break;

    case Direction::East:
        state_.direction = Direction::North;
        break;
    }

    return true;
}

bool VirtualRobot::turnRight()
{
    switch (state_.direction) {
    case Direction::North:
        state_.direction = Direction::East;
        break;

    case Direction::East:
        state_.direction = Direction::South;
        break;

    case Direction::South:
        state_.direction = Direction::West;
        break;

    case Direction::West:
        state_.direction = Direction::North;
        break;
    }

    return true;
}

bool VirtualRobot::turnAround()
{
    switch (state_.direction) {
    case Direction::North:
        state_.direction = Direction::South;
        break;

    case Direction::East:
        state_.direction = Direction::West;
        break;

    case Direction::South:
        state_.direction = Direction::North;
        break;

    case Direction::West:
        state_.direction = Direction::East;
        break;
    }

    return true;
}

} // namespace meshmerize