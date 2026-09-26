#include <cassert>
#include <vector>

#include "robot/orientation.h"
#include "exploration/explorer.h"
#include "exploration/exploration_trace.h"
#include "maze/maze_builder.h"
#include "maze/maze_map.h"
#include "perception/end_detector.h"
#include "perception/junction_detector.h"
#include "simulator/virtual_robot.h"
#include "simulator/virtual_sensor.h"

using namespace meshmerize;

static MazeMap buildExplorationMaze()
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
     * Actual exploration:
     *
     * 0 -> 1
     * 1 -> 2       dead end
     * 2 -> 1       backtrack
     * 1 -> 3
     * 3 -> 4       END
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
        100.0,
        100.0
    ));

    /*
     * Node 4 is EAST of node 3.
     *
     * This makes the final movement
     * 3 -> 4 a straight movement when
     * the robot is facing East.
     */
    assert(builder.addDiscoveredNode(
        4,
        200.0,
        100.0
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
        1,
        3,
        Direction::East,
        100.0
    ));

    assert(builder.connectNodes(
        3,
        4,
        Direction::East,
        100.0
    ));

    assert(maze.setEndNode(4));

    return maze;
}

static void turnRobot(
    VirtualRobot& robot,
    RelativeDirection direction
)
{
    switch (direction) {

    case RelativeDirection::Left:
        assert(robot.turnLeft());
        break;

    case RelativeDirection::Right:
        assert(robot.turnRight());
        break;

    case RelativeDirection::Back:
        assert(robot.turnAround());
        break;

    case RelativeDirection::Straight:
        break;
    }
}

static Action actionFromDirection(
    RelativeDirection direction
)
{
    switch (direction) {

    case RelativeDirection::Left:
        return Action::Left;

    case RelativeDirection::Right:
        return Action::Right;

    case RelativeDirection::Back:
        return Action::UTurn;

    case RelativeDirection::Straight:
        return Action::Forward;
    }

    return Action::Stop;
}

static bool branchExplored(
    const MazeMap& maze,
    const VirtualRobot& robot,
    RelativeDirection relative
)
{
    /*
     * Convert the robot-relative direction
     * into an absolute compass direction.
     */
    const Direction absolute =
        applyRelativeTurn(
            robot.state().direction,
            relative
        );

    const auto neighbors =
        maze.graph().getNeighbors(
            robot.state().node_id
        );

    for (const int neighbor : neighbors) {

        const Edge* edge =
            maze.graph().getEdge(
                robot.state().node_id,
                neighbor
            );

        if (edge == nullptr) {
            continue;
        }

        Direction edge_direction =
            edge->direction;

        /*
         * Graph edges are undirected.
         *
         * If the robot is currently at edge.to,
         * reverse the stored edge direction.
         */
        if (edge->to == robot.state().node_id) {
            edge_direction =
                turnAround(edge_direction);
        }

        if (edge_direction == absolute) {
            return edge->explored;
        }
    }

    /*
     * No physical branch exists.
     *
     * Treat it as unavailable.
     */
    return false;
}

static void moveAlongChosenBranch(
    VirtualRobot& robot,
    MazeMap& maze,
    RelativeDirection direction,
    ExplorationTrace& trace
)
{
    const int from =
        robot.state().node_id;

    /*
     * Convert the relative decision into
     * a physical robot turn.
     */
    turnRobot(
        robot,
        direction
    );

    const Action action =
        actionFromDirection(direction);

    trace.record(action);

    /*
     * Find the graph edge matching the
     * robot's new heading.
     */
    const Direction heading =
        robot.state().direction;

    const auto neighbors =
        maze.graph().getNeighbors(from);

    int target = -1;

    for (const int neighbor : neighbors) {

        const Edge* edge =
            maze.graph().getEdge(
                from,
                neighbor
            );

        if (edge == nullptr) {
            continue;
        }

        Direction edge_direction =
            edge->direction;

        /*
         * Reverse the stored direction when
         * traversing the edge backwards.
         */
        if (edge->to == from) {
            edge_direction =
                turnAround(edge_direction);
        }

        if (edge_direction == heading) {
            target = neighbor;
            break;
        }
    }

    assert(target != -1);

    const Edge* edge =
        maze.graph().getEdge(
            from,
            target
        );

    assert(edge != nullptr);

    /*
     * Move through the selected branch.
     */
    assert(
        robot.moveForward(
            edge->distance
        )
    );

    /*
     * Once traversed, the edge is explored.
     */
    assert(
        maze.graph().markEdgeExplored(
            from,
            target
        )
    );

    trace.record(Action::Forward);
}

static void test_complete_dry_run()
{
    MazeMap maze =
        buildExplorationMaze();

    VirtualRobot robot(maze);
    VirtualSensor sensor(robot);

    JunctionDetector junction_detector;
    EndZoneDetector end_detector;

    Explorer explorer;
    ExplorationTrace trace;

    explorer.reset();
    trace.reset();

    /*
     * =========================================
     * START — Node 0
     * =========================================
     */

    assert(robot.state().node_id == 0);

    JunctionObservation observation =
        junction_detector.detect(
            sensor.read()
        );

    /*
     * Start is a normal straight line.
     */
    assert(!observation.detected);

    /*
     * Follow line from node 0 -> node 1.
     */
    assert(
        robot.moveForward(100.0)
    );

    trace.record(Action::Forward);

    /*
     * Mark the traversed edge as explored.
     */
    assert(
        maze.graph().markEdgeExplored(0, 1)
    );

    /*
     * =========================================
     * NODE 1
     * =========================================
     *
     * Robot is facing North.
     *
     * Available:
     *
     * Left     = none
     * Straight = node 2
     * Right    = node 3
     */

    observation =
        junction_detector.detect(
            sensor.read()
        );

    assert(observation.detected);
    assert(!observation.left);
    assert(observation.straight);
    assert(observation.right);

    JunctionDecision decision =
        explorer.chooseDirection(
            observation,

            branchExplored(
                maze,
                robot,
                RelativeDirection::Straight
            ),

            branchExplored(
                maze,
                robot,
                RelativeDirection::Left
            ),

            branchExplored(
                maze,
                robot,
                RelativeDirection::Right
            )
        );

    /*
     * SLRB:
     *
     * Straight has priority because
     * node 1 -> node 2 is unexplored.
     */
    assert(
        decision.direction ==
        RelativeDirection::Straight
    );

    assert(!decision.should_backtrack);

    assert(
        explorer.enterBranch(
            robot.state().node_id,
            decision.direction
        )
    );

    moveAlongChosenBranch(
        robot,
        maze,
        decision.direction,
        trace
    );

    /*
     * =========================================
     * NODE 2 — DEAD END
     * =========================================
     */

    assert(robot.state().node_id == 2);

    observation =
        junction_detector.detect(
            sensor.read()
        );

    /*
     * There is no left/right/straight branch.
     *
     * The current Phase 24 test explicitly
     * handles the dead end by choosing Back.
     */
    assert(!observation.detected);

    decision =
        explorer.chooseDirection(
            observation,
            true,
            true,
            true
        );

    assert(
        decision.direction ==
        RelativeDirection::Back
    );

    assert(decision.should_backtrack);

    /*
     * Physical U-turn.
     */
    assert(
        robot.turnAround()
    );

    trace.record(Action::UTurn);

    /*
     * Pop the exploration frame.
     */
    ExplorationFrame frame;

    assert(
        explorer.backtrack(frame)
    );

    /*
     * Return node 2 -> node 1.
     */
    assert(
        robot.moveForward(100.0)
    );

    trace.record(Action::Forward);

    assert(
        maze.graph().markEdgeExplored(2, 1)
    );

    assert(
        robot.state().node_id == 1
    );

    /*
     * =========================================
     * NODE 1 — SECOND VISIT
     * =========================================
     *
     * Robot is now facing South.
     *
     * Physical branches:
     *
     * South = node 0
     * East  = node 3
     *
     * Relative to the robot:
     *
     * Straight = South = node 0
     * Left     = East  = node 3
     * Right    = West  = no branch
     *
     * node 0 is already explored.
     * node 3 is unexplored.
     *
     * Therefore SLRB must choose LEFT.
     */

    observation =
        junction_detector.detect(
            sensor.read()
        );

    assert(observation.detected);

    /*
     * Verify the actual sensor geometry.
     */
    assert(observation.left);
    assert(observation.straight);
    assert(!observation.right);

    /*
     * IMPORTANT:
     *
     * Do not manually provide exploration flags.
     *
     * Ask the graph which branches are
     * actually explored.
     */
    decision =
        explorer.chooseDirection(
            observation,

            branchExplored(
                maze,
                robot,
                RelativeDirection::Straight
            ),

            branchExplored(
                maze,
                robot,
                RelativeDirection::Left
            ),

            branchExplored(
                maze,
                robot,
                RelativeDirection::Right
            )
        );

    /*
     * Straight is already explored.
     *
     * Left leads to unexplored node 3.
     *
     * Therefore SLRB chooses Left.
     */
    assert(
        decision.direction ==
        RelativeDirection::Left
    );

    assert(!decision.should_backtrack);

    assert(
        explorer.enterBranch(
            robot.state().node_id,
            decision.direction
        )
    );

    moveAlongChosenBranch(
        robot,
        maze,
        decision.direction,
        trace
    );

    /*
     * =========================================
     * NODE 3
     * =========================================
     *
     * Robot is now facing East.
     *
     * Node 4 is directly ahead.
     */

    assert(
        robot.state().node_id == 3
    );

    observation =
        junction_detector.detect(
            sensor.read()
        );

    /*
     * Straight line toward node 4.
     */
    assert(!observation.detected);

    /*
     * END-zone detection requires the detector
     * to have previously seen a normal line.
     *
     * The robot is currently still on the line
     * at node 3, so feed this reading to the
     * EndZoneDetector before entering the END zone.
     */
    assert(
        !end_detector.detect(
            sensor.read()
        )
    );

    /*
     * Move node 3 -> node 4.
     */
    assert(
        robot.moveForward(100.0)
    );

    trace.record(Action::Forward);

    assert(
        maze.graph().markEdgeExplored(3, 4)
    );

    /*
     * =========================================
     * END NODE 4
     * =========================================
     */

    assert(
        robot.state().node_id == 4
    );

    assert(
        robot.isAtEndNode()
    );

    /*
     * EndZoneDetector requires three
     * consecutive wide-white confirmations.
     */
    assert(
        !end_detector.detect(
            sensor.read()
        )
    );

    assert(
        !end_detector.detect(
            sensor.read()
        )
    );

    assert(
        end_detector.detect(
            sensor.read()
        )
    );

    /*
     * =========================================
     * VERIFY COMPLETE TRACE
     * =========================================
     *
     * Expected:
     *
     * 0 -> 1       Forward
     * 1 -> 2       Forward
     * 2 -> 1       UTurn + Forward
     * 1 -> 3       Left + Forward
     * 3 -> 4       Forward
     *
     * Trace:
     *
     * [Forward,
     *  Forward,
     *  UTurn,
     *  Forward,
     *  Left,
     *  Forward,
     *  Forward]
     */

    const std::vector<Action>& actions =
        trace.actions();

    /*
     * The trace records both:
     *
     * 1. the selected relative action
     * 2. the actual Forward movement
     *
     * Therefore the complete trace is:
     *
     * [Forward,        // 0 -> 1
     *  Forward,        // choose Straight
     *  Forward,        // 1 -> 2
     *  UTurn,          // turn around at dead end
     *  Forward,        // 2 -> 1
     *  Left,            // choose Left
     *  Forward,        // 1 -> 3
     *  Forward]        // 3 -> 4
     */

    assert(actions.size() == 8);

    assert(
        actions[0] ==
        Action::Forward
    );

    assert(
        actions[1] ==
        Action::Forward
    );

    assert(
        actions[2] ==
        Action::Forward
    );

    assert(
        actions[3] ==
        Action::UTurn
    );

    assert(
        actions[4] ==
        Action::Forward
    );

    assert(
        actions[5] ==
        Action::Left
    );

    assert(
        actions[6] ==
        Action::Forward
    );

    assert(
        actions[7] ==
        Action::Forward
    );

}

int main()
{
    test_complete_dry_run();

    return 0;
}