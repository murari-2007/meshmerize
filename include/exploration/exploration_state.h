#pragma once

#include <array>
#include <cstddef>

#include "core/types.h"

namespace meshmerize {

struct ExplorationFrame {
    int node_id = -1;

    // Heading when the robot left this node.
    Direction exit_heading = Direction::North;
};

class ExplorationState {
public:
    static constexpr std::size_t MAX_DEPTH = 100;

    void reset();

    bool push(
        int node_id,
        Direction exit_heading
    );

    bool pop(
        ExplorationFrame& frame
    );

    bool empty() const;

    std::size_t depth() const;

    Direction heading() const;

    void setHeading(Direction heading);

    void updateHeading(RelativeDirection turn);

private:
    std::array<ExplorationFrame, MAX_DEPTH> stack_{};

    std::size_t stack_size_ = 0;

    Direction heading_ = Direction::North;
};

} // namespace meshmerize