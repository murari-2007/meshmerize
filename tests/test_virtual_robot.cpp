#include <cassert>

#include "maze/edge.h"
#include "maze/node.h"
#include "maze/maze_map.h"
#include "simulator/virtual_robot.h"

int main()
{
    using namespace meshmerize;

    MazeMap maze;

    Node start;
    start.id = 0;
    start.x = 0.0;
    start.y = 0.0;
    start.is_start = true;

    Node junction;
    junction.id = 1;
    junction.x = 100.0;
    junction.y = 0.0;

    Node end;
    end.id = 2;
    end.x = 200.0;
    end.y = 0.0;
    end.is_end = true;

    assert(maze.graph().addNode(start));
    assert(maze.graph().addNode(junction));
    assert(maze.graph().addNode(end));

    Edge edge_01;
    edge_01.from = 0;
    edge_01.to = 1;
    edge_01.distance = 100.0;
    edge_01.direction = Direction::East;

    Edge edge_12;
    edge_12.from = 1;
    edge_12.to = 2;
    edge_12.distance = 100.0;
    edge_12.direction = Direction::East;

    assert(maze.graph().addEdge(edge_01));
    assert(maze.graph().addEdge(edge_12));

    assert(maze.setStartNode(0));
    assert(maze.setEndNode(2));

    VirtualRobot robot(maze);

    // Initial state.
    assert(robot.state().node_id == 0);
    assert(robot.state().x == 0.0);
    assert(robot.state().y == 0.0);
    assert(robot.state().direction == Direction::North);

    // No road exists north of the start.
    assert(!robot.moveForward(100.0));

    // Turn toward the road.
    assert(robot.turnRight());

    assert(robot.state().direction == Direction::East);

    // Move from node 0 -> node 1.
    assert(robot.moveForward(100.0));

    assert(robot.state().node_id == 1);
    assert(robot.state().x == 100.0);
    assert(robot.state().y == 0.0);

    assert(robot.state().left_encoder == 100.0);
    assert(robot.state().right_encoder == 100.0);

    // Continue from node 1 -> node 2.
    assert(robot.moveForward(100.0));

    assert(robot.state().node_id == 2);
    assert(robot.state().x == 200.0);
    assert(robot.state().y == 0.0);

    assert(robot.state().left_encoder == 200.0);
    assert(robot.state().right_encoder == 200.0);

    // Test turning.
    assert(robot.turnLeft());
    assert(robot.state().direction == Direction::North);

    assert(robot.turnLeft());
    assert(robot.state().direction == Direction::West);

    assert(robot.turnAround());
    assert(robot.state().direction == Direction::East);

    return 0;
}