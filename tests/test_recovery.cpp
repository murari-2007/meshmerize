#include <cassert>

#include "execution/recovery.h"

using namespace meshmerize;

static void test_initial_state()
{
    RecoveryManager manager;

    assert(manager.state() ==
           RecoveryState::Normal);

    assert(manager.action() ==
           RecoveryAction::Continue);

    assert(!manager.inRecovery());
    assert(!manager.faulted());
    assert(!manager.completed());
}

static void test_line_lost()
{
    RecoveryManager manager;

    manager.reportLineLost();

    assert(manager.state() ==
           RecoveryState::LineLost);

    assert(manager.action() ==
           RecoveryAction::RecoverLine);

    assert(manager.inRecovery());
    assert(!manager.faulted());
}

static void test_junction_missed()
{
    RecoveryManager manager;

    manager.reportJunctionMissed();

    assert(manager.state() ==
           RecoveryState::JunctionMissed);

    assert(manager.action() ==
           RecoveryAction::RecoverJunction);

    assert(manager.inRecovery());
}

static void test_turn_failed()
{
    RecoveryManager manager;

    manager.reportTurnFailed();

    assert(manager.state() ==
           RecoveryState::TurnFailed);

    assert(manager.action() ==
           RecoveryAction::RetryTurn);

    assert(manager.inRecovery());
}

static void test_encoder_error()
{
    RecoveryManager manager;

    manager.reportEncoderError();

    assert(manager.state() ==
           RecoveryState::EncoderError);

    assert(manager.action() ==
           RecoveryAction::Fault);

    assert(manager.faulted());
}

static void test_timeout()
{
    RecoveryManager manager;

    manager.reportTimeout();

    assert(manager.state() ==
           RecoveryState::Timeout);

    assert(manager.action() ==
           RecoveryAction::Stop);

    assert(manager.faulted());
}

static void test_end_detected()
{
    RecoveryManager manager;

    manager.reportEndDetected();

    assert(manager.state() ==
           RecoveryState::EndDetected);

    assert(manager.action() ==
           RecoveryAction::Complete);

    assert(manager.completed());
    assert(!manager.faulted());
}

static void test_reset()
{
    RecoveryManager manager;

    manager.reportLineLost();

    assert(manager.inRecovery());

    manager.reset();

    assert(manager.state() ==
           RecoveryState::Normal);

    assert(manager.action() ==
           RecoveryAction::Continue);
}

static void test_fault_cannot_become_end()
{
    RecoveryManager manager;

    manager.reportEncoderError();

    manager.reportEndDetected();

    assert(manager.state() ==
           RecoveryState::EncoderError);

    assert(manager.faulted());
}

static void test_end_cannot_become_line_lost()
{
    RecoveryManager manager;

    manager.reportEndDetected();

    manager.reportLineLost();

    assert(manager.state() ==
           RecoveryState::EndDetected);

    assert(manager.completed());
}

static void test_clear_fault()
{
    RecoveryManager manager;

    manager.reportEncoderError();

    assert(manager.faulted());

    manager.clearFault();

    assert(manager.state() ==
           RecoveryState::Normal);

    assert(!manager.faulted());
}

int main()
{
    test_initial_state();
    test_line_lost();
    test_junction_missed();
    test_turn_failed();
    test_encoder_error();
    test_timeout();
    test_end_detected();
    test_reset();
    test_fault_cannot_become_end();
    test_end_cannot_become_line_lost();
    test_clear_fault();

    return 0;
}