#pragma once

namespace meshmerize {

struct EncoderReading {
    double left_ticks = 0.0;
    double right_ticks = 0.0;
};

class EncoderInterface {
public:
    virtual ~EncoderInterface() = default;

    virtual EncoderReading read() = 0;

    virtual void reset() = 0;
};

} // namespace meshmerize
