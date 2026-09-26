#pragma once

#include "core/types.h"

namespace meshmerize {

Direction turnLeft(Direction direction);

Direction turnRight(Direction direction);

Direction turnAround(Direction direction);

Direction applyRelativeTurn(
    Direction direction,
    RelativeDirection turn
);

} // namespace meshmerize