#include "robot/orientation.h"

namespace meshmerize {

namespace {

Direction fromIndex(int index)
{
    index %= 4;

    if (index < 0) {
        index += 4;
    }

    return static_cast<Direction>(index);
}

int toIndex(Direction direction)
{
    return static_cast<int>(direction);
}

} // namespace

Direction turnLeft(Direction direction)
{
    return fromIndex(toIndex(direction) - 1);
}

Direction turnRight(Direction direction)
{
    return fromIndex(toIndex(direction) + 1);
}

Direction turnAround(Direction direction)
{
    return fromIndex(toIndex(direction) + 2);
}

Direction applyRelativeTurn(
    Direction direction,
    RelativeDirection turn
)
{
    switch (turn) {
    case RelativeDirection::Left:
        return turnLeft(direction);

    case RelativeDirection::Straight:
        return direction;

    case RelativeDirection::Right:
        return turnRight(direction);

    case RelativeDirection::Back:
        return turnAround(direction);
    }

    // Defensive fallback.
    return direction;
}

} // namespace meshmerize