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

    return 0;
}