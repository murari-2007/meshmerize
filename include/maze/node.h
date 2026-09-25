#pragma once

namespace meshmerize {

struct Node {
    int id = -1;

    double x = 0.0;
    double y = 0.0;

    bool is_start = false;
    bool is_end = false;
};

} // namespace meshmerize