#include <cassert>
#include <vector>

#include "execution/route_executor.h"
#include "maze/maze_builder.h"
#include "maze/maze_map.h"
#include "planning/dijkstra.h"
#include "planning/path_optimizer.h"
#include "planning/route_builder.h"
#include "simulator/virtual_robot.h"

using namespace meshmerize;

static MazeMap buildMaze()
{
    MazeMap maze;
    MazeBuilder builder(maze);

    /*
     * Maze:
     *
     *             2
     *             |
     *             |
     *             1 ---- 3 ---- 4 END
     *             |
     *             |
     *             0 START
     *
     * Longer:
     *
     * 0 -> 1 -> 2 -> 3 -> 4
     *
     * Shorter:
     *
     * 0 -> 1 -> 3 -> 4
     */

    assert(
        builder.addStartNode(
            0,
            0.0,
            0.0
        )
    );

    assert(
        builder.addDiscoveredNode(
            1,
            0.0,
            100.0
        )
    );

    assert(
        builder.addDiscoveredNode(
            2,
            0.0,
            200.0
        )
    );

    assert(
        builder.addDiscoveredNode(
            3,
            100.0,
            100.0
        )
    );

    assert(
        builder.addDiscoveredNode(
            4,
            200.0,
            100.0
        )
    );

    assert(
        builder.connectNodes(
            0,
            1,
            Direction::North,
            100.0
        )
    );

    assert(
        builder.connectNodes(
            1,
            2,
            Direction::North,
            100.0
        )
    );

    assert(
        builder.connectNodes(
            2,
            3,
            Direction::East,
            141.0
        )
    );

    assert(
        builder.connectNodes(
            1,
            3,
            Direction::East,
            100.0
        )
    );

    assert(
        builder.connectNodes(
            3,
            4,
            Direction::East,
            100.0
        )
    );

    assert(
        maze.setEndNode(4)
    );

    return maze;
}

static double edgeDistance(
    const MazeMap& maze,
    int from,
    Direction direction
)
{
    const auto neighbors =
        maze.graph().getNeighbors(from);

    for (const int neighbor : neighbors) {

        const Edge* edge =
            maze.graph().getEdge(
                from,
                neighbor
            );

        assert(edge != nullptr);

        Direction edge_direction =
            edge->direction;

        if (edge->to == from) {

            switch (edge_direction) {

            case Direction::North:
                edge_direction =
                    Direction::South;
                break;

            case Direction::East:
                edge_direction =
                    Direction::West;
                break;

            case Direction::South:
                edge_direction =
                    Direction::North;
                break;

            case Direction::West:
                edge_direction =
                    Direction::East;
                break;
            }
        }

        if (edge_direction == direction) {
            return edge->distance;
        }
    }

    return -1.0;
}

static void applyTurn(
    VirtualRobot& robot,
    Action action
)
{
    switch (action) {

    case Action::Left:
        assert(robot.turnLeft());
        break;

    case Action::Right:
        assert(robot.turnRight());
        break;

    case Action::UTurn:
        assert(robot.turnAround());
        break;

    case Action::Forward:
    case Action::Stop:
        break;
    }
}

static void test_full_shortest_route_execution()
{
    MazeMap maze =
        buildMaze();

    /*
     * -----------------------------------------
     * Planning
     * -----------------------------------------
     */

    const std::vector<int> node_path =
        dijkstra(
            maze.graph(),
            maze.startNode(),
            maze.endNode()
        );

    /*
     * The weighted shortest path must avoid
     * node 2.
     */
    assert(node_path.size() == 4);

    assert(node_path[0] == 0);
    assert(node_path[1] == 1);
    assert(node_path[2] == 3);
    assert(node_path[3] == 4);

    RouteBuilder builder;

    const Route raw_route =
        builder.build(
            maze.graph(),
            node_path,
            Direction::North
        );

    /*
     * Expected:
     *
     * Forward
     * Right
     * Forward
     * Forward
     */
    assert(raw_route.size() == 4);

    assert(
        raw_route.actions[0] ==
        Action::Forward
    );

    assert(
        raw_route.actions[1] ==
        Action::Right
    );

    assert(
        raw_route.actions[2] ==
        Action::Forward
    );

    assert(
        raw_route.actions[3] ==
        Action::Forward
    );

    PathOptimizer optimizer;

    const Route route =
        optimizer.optimize(
            raw_route
        );

    /*
     * Final executable route:
     *
     * Forward
     * Right
     * Forward
     * Forward
     * Stop
     */
    assert(route.size() == 5);

    assert(
        route.actions[4] ==
        Action::Stop
    );

    /*
     * -----------------------------------------
     * Actual-run simulation
     * -----------------------------------------
     */

    VirtualRobot robot(maze);

    /*
     * target_ticks_90 is deliberately a
     * simulation/test value.
     */
    TurnConfig turn_config;

    turn_config.target_ticks_90 =
        100.0;

    turn_config.tolerance_ticks =
        3.0;

    turn_config.turn_speed =
        0.5;

    RouteExecutor executor(
        turn_config
    );

    executor.loadRoute(route);
    executor.start();

    assert(
        executor.active() ||
        executor.state() ==
            RouteExecutorState::Idle
    );

    /*
     * Simulated encoder values used only for
     * exercising TurnController.
     */
    double left_encoder = 0.0;
    double right_encoder = 0.0;

    std::size_t guard = 0;

    while (!executor.completed()) {

        assert(
            executor.state() !=
            RouteExecutorState::Fault
        );

        assert(
            guard++ < 100
        );

        RouteExecutorFeedback feedback;

        feedback.turn_feedback.left_encoder =
            left_encoder;

        feedback.turn_feedback.right_encoder =
            right_encoder;

        /*
         * -------------------------------------
         * Forward
         * -------------------------------------
         */
        if (
            executor.state() ==
            RouteExecutorState::Forward
        ) {
            const int current_node =
                robot.state().node_id;

            const Direction heading =
                robot.state().direction;

            const double distance =
                edgeDistance(
                    maze,
                    current_node,
                    heading
                );

            assert(distance > 0.0);

            /*
             * The virtual robot represents the
             * completed physical movement.
             */
            assert(
                robot.moveForward(distance)
            );

            feedback.forward_complete =
                true;

            executor.update(feedback);

            continue;
        }

        /*
         * -------------------------------------
         * Turning
         * -------------------------------------
         */
        if (
            executor.state() ==
            RouteExecutorState::Turning
        ) {
            /*
             * Apply the physical turn once.
             *
             * RouteExecutor still owns the
             * turn-control state.
             */
            applyTurn(
                robot,
                executor.currentAction()
            );

            /*
             * Simulate encoder progress toward
             * the configured 90-degree target.
             *
             * Update the encoder values BEFORE
             * constructing the feedback object so
             * TurnController receives the new
             * encoder position.
             */
            left_encoder += 100.0;
            right_encoder += 100.0;

            feedback.turn_feedback.left_encoder =
                left_encoder;

            feedback.turn_feedback.right_encoder =
                right_encoder;

            executor.update(feedback);

            continue;
        }

        /*
         * -------------------------------------
         * Idle
         * -------------------------------------
         */
        executor.update(feedback);
    }

    /*
     * -----------------------------------------
     * Final validation
     * -----------------------------------------
     */

    assert(
        executor.state() ==
        RouteExecutorState::Completed
    );

    assert(
        robot.state().node_id ==
        maze.endNode()
    );

    assert(
        robot.state().x ==
        200.0
    );

    assert(
        robot.state().y ==
        100.0
    );

    assert(
        robot.state().direction ==
        Direction::East
    );

    /*
     * The robot must have avoided node 2.
     */
    assert(
        robot.state().node_id != 2
    );
}

int main()
{
    test_full_shortest_route_execution();

    return 0;
}
