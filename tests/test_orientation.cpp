#include <cassert>

#include "robot/orientation.h"

int main()
{
    using namespace meshmerize;

    // --------------------------------------------------
    // Right turns
    // --------------------------------------------------

    assert(turnRight(Direction::North) == Direction::East);
    assert(turnRight(Direction::East) == Direction::South);
    assert(turnRight(Direction::South) == Direction::West);
    assert(turnRight(Direction::West) == Direction::North);

    // --------------------------------------------------
    // Left turns
    // --------------------------------------------------

    assert(turnLeft(Direction::North) == Direction::West);
    assert(turnLeft(Direction::West) == Direction::South);
    assert(turnLeft(Direction::South) == Direction::East);
    assert(turnLeft(Direction::East) == Direction::North);

    // --------------------------------------------------
    // U-turns
    // --------------------------------------------------

    assert(turnAround(Direction::North) == Direction::South);
    assert(turnAround(Direction::East) == Direction::West);
    assert(turnAround(Direction::South) == Direction::North);
    assert(turnAround(Direction::West) == Direction::East);

    // --------------------------------------------------
    // Straight
    // --------------------------------------------------

    assert(
        applyRelativeTurn(
            Direction::North,
            RelativeDirection::Straight
        ) == Direction::North
    );

    assert(
        applyRelativeTurn(
            Direction::East,
            RelativeDirection::Straight
        ) == Direction::East
    );

    // --------------------------------------------------
    // Relative turns from North
    // --------------------------------------------------

    assert(
        applyRelativeTurn(
            Direction::North,
            RelativeDirection::Left
        ) == Direction::West
    );

    assert(
        applyRelativeTurn(
            Direction::North,
            RelativeDirection::Right
        ) == Direction::East
    );

    assert(
        applyRelativeTurn(
            Direction::North,
            RelativeDirection::Back
        ) == Direction::South
    );

    // --------------------------------------------------
    // Relative turns from East
    // --------------------------------------------------

    assert(
        applyRelativeTurn(
            Direction::East,
            RelativeDirection::Left
        ) == Direction::North
    );

    assert(
        applyRelativeTurn(
            Direction::East,
            RelativeDirection::Right
        ) == Direction::South
    );

    assert(
        applyRelativeTurn(
            Direction::East,
            RelativeDirection::Back
        ) == Direction::West
    );

    // --------------------------------------------------
    // Relative turns from South
    // --------------------------------------------------

    assert(
        applyRelativeTurn(
            Direction::South,
            RelativeDirection::Left
        ) == Direction::East
    );

    assert(
        applyRelativeTurn(
            Direction::South,
            RelativeDirection::Right
        ) == Direction::West
    );

    // --------------------------------------------------
    // Relative turns from West
    // --------------------------------------------------

    assert(
        applyRelativeTurn(
            Direction::West,
            RelativeDirection::Left
        ) == Direction::South
    );

    assert(
        applyRelativeTurn(
            Direction::West,
            RelativeDirection::Right
        ) == Direction::North
    );

    return 0;
}