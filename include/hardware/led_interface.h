#pragma once

namespace meshmerize {

class LedInterface {
public:
    virtual ~LedInterface() = default;

    virtual void set(bool enabled) = 0;

    virtual void on() = 0;

    virtual void off() = 0;
};

} // namespace meshmerize
