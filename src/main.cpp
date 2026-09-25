#include <iostream>

#include "core/config.h"
#include "core/types.h"
#include "planning/route.h"
#include "robot/robot_state.h"

int main() {
    using namespace meshmerize;

    RobotState robot;

    std::cout << "Meshmerize Robot Software\n";
    std::cout << "Version: 0.1.0\n";
    std::cout << "Status: INITIALIZED\n";

    std::cout << "\nRobot state:\n";
    std::cout << "Node: " << robot.node_id << '\n';
    std::cout << "Position: (" << robot.x
              << ", " << robot.y << ")\n";

    return 0;
}