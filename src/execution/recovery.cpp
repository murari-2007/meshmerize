#include "execution/recovery.h"

namespace meshmerize {

void RecoveryManager::reset()
{
    state_ = RecoveryState::Normal;
}

void RecoveryManager::reportLineLost()
{
    if (state_ == RecoveryState::EndDetected ||
        state_ == RecoveryState::Fault) {
        return;
    }

    state_ = RecoveryState::LineLost;
}

void RecoveryManager::reportJunctionMissed()
{
    if (state_ == RecoveryState::EndDetected ||
        state_ == RecoveryState::Fault) {
        return;
    }

    state_ = RecoveryState::JunctionMissed;
}

void RecoveryManager::reportTurnFailed()
{
    if (state_ == RecoveryState::EndDetected ||
        state_ == RecoveryState::Fault) {
        return;
    }

    state_ = RecoveryState::TurnFailed;
}

void RecoveryManager::reportEncoderError()
{
    state_ = RecoveryState::EncoderError;
}

void RecoveryManager::reportTimeout()
{
    state_ = RecoveryState::Timeout;
}

void RecoveryManager::reportEndDetected()
{
    if (state_ == RecoveryState::EncoderError ||
        state_ == RecoveryState::Timeout ||
        state_ == RecoveryState::Fault) {
        return;
    }

    state_ = RecoveryState::EndDetected;
}

void RecoveryManager::clearFault()
{
    if (state_ == RecoveryState::EncoderError ||
        state_ == RecoveryState::Timeout ||
        state_ == RecoveryState::Fault) {
        state_ = RecoveryState::Normal;
    }
}

RecoveryState RecoveryManager::state() const
{
    return state_;
}

RecoveryAction RecoveryManager::action() const
{
    switch (state_) {

        case RecoveryState::Normal:
            return RecoveryAction::Continue;

        case RecoveryState::LineLost:
            return RecoveryAction::RecoverLine;

        case RecoveryState::JunctionMissed:
            return RecoveryAction::RecoverJunction;

        case RecoveryState::TurnFailed:
            return RecoveryAction::RetryTurn;

        case RecoveryState::EncoderError:
            return RecoveryAction::Fault;

        case RecoveryState::Timeout:
            return RecoveryAction::Stop;

        case RecoveryState::EndDetected:
            return RecoveryAction::Complete;

        case RecoveryState::Fault:
            return RecoveryAction::Fault;
    }

    return RecoveryAction::Fault;
}

bool RecoveryManager::inRecovery() const
{
    return state_ == RecoveryState::LineLost ||
           state_ == RecoveryState::JunctionMissed ||
           state_ == RecoveryState::TurnFailed;
}

bool RecoveryManager::faulted() const
{
    return state_ == RecoveryState::EncoderError ||
           state_ == RecoveryState::Timeout ||
           state_ == RecoveryState::Fault;
}

bool RecoveryManager::completed() const
{
    return state_ == RecoveryState::EndDetected;
}

} // namespace meshmerize