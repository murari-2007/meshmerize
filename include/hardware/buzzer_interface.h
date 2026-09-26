#pragma once

namespace meshmerize {

class BuzzerInterface {
public:
    virtual ~BuzzerInterface() = default;

    virtual void on() = 0;

    virtual void off() = 0;

    virtual void beep(unsigned int duration_ms) = 0;
};

} // namespace meshmerize
