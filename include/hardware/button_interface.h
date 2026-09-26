#pragma once

namespace meshmerize {

class ButtonInterface {
public:
    virtual ~ButtonInterface() = default;

    virtual bool isPressed() = 0;
};

} // namespace meshmerize
