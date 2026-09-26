#pragma once

#include <cstdint>

namespace meshmerize {

enum class RecoveryState : std::uint8_t {
    Normal = 0,
    LineLost,
    JunctionMissed,
    TurnFailed,
    EncoderError,
    Timeout,
    EndDetected,
    Fault
};

enum class RecoveryAction : std::uint8_t {
    Continue = 0,
    RecoverLine,
    RecoverJunction,
    RetryTurn,
    Stop,
    Complete,
    Fault
};

class RecoveryManager {
public:
    void reset();

    void reportLineLost();
    void reportJunctionMissed();
    void reportTurnFailed();
    void reportEncoderError();
    void reportTimeout();
    void reportEndDetected();

    void clearFault();

    RecoveryState state() const;

    RecoveryAction action() const;

    bool inRecovery() const;

    bool faulted() const;

    bool completed() const;

private:
    RecoveryState state_ = RecoveryState::Normal;
};

} // namespace meshmerize