#include <cassert>

#include "exploration/explorer.h"

int main()
{
    using namespace meshmerize;

    Explorer explorer;

    // --------------------------------------------------
    // Reset state
    // --------------------------------------------------

    explorer.reset();

    assert(explorer.heading() == Direction::North);
    assert(explorer.depth() == 0);

    // --------------------------------------------------
    // SLRB: Straight has highest priority
    // --------------------------------------------------

    JunctionObservation junction;

    junction.detected = true;
    junction.left = true;
    junction.straight = true;
    junction.right = true;

    JunctionDecision decision =
        explorer.chooseDirection(
            junction,
            false,
            false,
            false
        );

    assert(
        decision.direction ==
        RelativeDirection::Straight
    );

    assert(!decision.should_backtrack);

    // --------------------------------------------------
    // SLRB: Left after Straight is explored
    // --------------------------------------------------

    decision =
        explorer.chooseDirection(
            junction,
            true,
            false,
            false
        );

    assert(
        decision.direction ==
        RelativeDirection::Left
    );

    assert(!decision.should_backtrack);

    // --------------------------------------------------
    // SLRB: Right after Straight + Left
    // --------------------------------------------------

    decision =
        explorer.chooseDirection(
            junction,
            true,
            true,
            false
        );

    assert(
        decision.direction ==
        RelativeDirection::Right
    );

    assert(!decision.should_backtrack);

    // --------------------------------------------------
    // SLRB: Backtrack when everything is explored
    // --------------------------------------------------

    decision =
        explorer.chooseDirection(
            junction,
            true,
            true,
            true
        );

    assert(
        decision.direction ==
        RelativeDirection::Back
    );

    assert(decision.should_backtrack);

    // --------------------------------------------------
    // DFS stack
    // --------------------------------------------------

    assert(
        explorer.enterBranch(
            0,
            RelativeDirection::Straight
        )
    );

    assert(explorer.depth() == 1);
    assert(explorer.heading() == Direction::North);

    assert(
        explorer.enterBranch(
            1,
            RelativeDirection::Right
        )
    );

    assert(explorer.depth() == 2);
    assert(explorer.heading() == Direction::East);

    // --------------------------------------------------
    // Backtrack
    // --------------------------------------------------

    ExplorationFrame frame;

    assert(explorer.backtrack(frame));

    assert(frame.node_id == 1);
    assert(
        frame.exit_heading ==
        Direction::East
    );

    assert(explorer.depth() == 1);

    assert(explorer.backtrack(frame));

    assert(frame.node_id == 0);
    assert(
        frame.exit_heading ==
        Direction::North
    );

    assert(explorer.depth() == 0);

    // --------------------------------------------------
    // Empty stack
    // --------------------------------------------------

    assert(!explorer.backtrack(frame));

        // --------------------------------------------------
    // Integration: simulated exploration sequence
    //
    // Maze idea:
    //
    // Start
    //   |
    //   S
    //   |
    //  Node 1
    //  /    \
    // L      R
    // |      |
    //Dead    Node 3
    //
    // At the dead end, the explorer backtracks
    // and later chooses the unexplored Right branch.
    // --------------------------------------------------

    explorer.reset();

    // Start at Node 0.
    // All three branches are available.
    // SLRB => Straight.
    junction.left = true;
    junction.straight = true;
    junction.right = true;

    decision =
        explorer.chooseDirection(
            junction,
            false,  // straight unexplored
            false,  // left unexplored
            false   // right unexplored
        );

    assert(
        decision.direction ==
        RelativeDirection::Straight
    );

    assert(
        explorer.enterBranch(
            0,
            decision.direction
        )
    );

    assert(explorer.heading() == Direction::North);
    assert(explorer.depth() == 1);

    // --------------------------------------------------
    // Node 1
    // Straight is already explored.
    // SLRB => Left.
    // --------------------------------------------------

    decision =
        explorer.chooseDirection(
            junction,
            true,   // straight explored
            false,  // left unexplored
            true    // right already explored for this simulation
        );

    assert(
        decision.direction ==
        RelativeDirection::Left
    );

    assert(
        explorer.enterBranch(
            1,
            decision.direction
        )
    );

    assert(explorer.heading() == Direction::West);
    assert(explorer.depth() == 2);

    // --------------------------------------------------
    // Dead end
    // No new branches available.
    // SLRB => Back.
    // --------------------------------------------------

    JunctionObservation dead_end;

    dead_end.detected = true;
    dead_end.left = false;
    dead_end.straight = false;
    dead_end.right = false;

    decision =
        explorer.chooseDirection(
            dead_end,
            true,
            true,
            true
        );

    assert(
        decision.direction ==
        RelativeDirection::Back
    );

    assert(decision.should_backtrack);

    // --------------------------------------------------
    // Backtrack from the dead end.
    // --------------------------------------------------



    assert(explorer.backtrack(frame));

    assert(frame.node_id == 1);
    assert(frame.exit_heading == Direction::West);
    assert(explorer.depth() == 1);

    // --------------------------------------------------
    // At Node 1 again.
    //
    // Straight and Left are explored.
    // Right is unexplored.
    //
    // SLRB => Right.
    // --------------------------------------------------

    decision =
        explorer.chooseDirection(
            junction,
            true,   // straight explored
            true,   // left explored
            false   // right unexplored
        );

    assert(
        decision.direction ==
        RelativeDirection::Right
    );

    assert(!decision.should_backtrack);

    assert(
        explorer.enterBranch(
            1,
            decision.direction
        )
    );

    // We were facing West.
    // Right from West = North.
    assert(explorer.heading() == Direction::North);

    assert(explorer.depth() == 2);

    return 0;
}