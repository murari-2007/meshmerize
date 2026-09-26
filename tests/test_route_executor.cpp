#include <cassert>

#include "execution/route_executor.h"

using namespace meshmerize;

static TurnConfig testConfig()
{
    TurnConfig config;

    config.target_ticks_90 = 100.0;
    config.tolerance_ticks = 3.0;
    config.turn_speed = 0.5;

    return config;
}

static void test_route_starts()
{
    RouteExecutor executor(testConfig());

    Route route;

    route.actions = {
        Action::Forward,
        Action::Stop
    };

    executor.loadRoute(route);

    assert(executor.state() ==
           RouteExecutorState::Idle);

    executor.start();

    assert(executor.currentActionIndex() == 0);
    assert(executor.currentAction() ==
           Action::Forward);
}

static void test_forward_action_becomes_active()
{
    RouteExecutor executor(testConfig());

    Route route;

    route.actions = {
        Action::Forward,
        Action::Stop
    };

    executor.loadRoute(route);
    executor.start();

    RouteExecutorFeedback feedback;

    executor.update(feedback);

    assert(executor.state() ==
           RouteExecutorState::Forward);

    assert(executor.active());
}

static void test_right_turn_starts_turn_controller()
{
    RouteExecutor executor(testConfig());

    Route route;

    route.actions = {
        Action::Right,
        Action::Stop
    };

    executor.loadRoute(route);
    executor.start();

    RouteExecutorFeedback feedback;

    feedback.turn_feedback.left_encoder = 0.0;
    feedback.turn_feedback.right_encoder = 0.0;

    executor.update(feedback);

    assert(executor.state() ==
           RouteExecutorState::Turning);

    assert(executor.currentAction() ==
           Action::Right);
}

static void test_turn_advances_route()
{
    RouteExecutor executor(testConfig());

    Route route;

    route.actions = {
        Action::Right,
        Action::Stop
    };

    executor.loadRoute(route);
    executor.start();

    RouteExecutorFeedback start;

    start.turn_feedback.left_encoder = 0.0;
    start.turn_feedback.right_encoder = 0.0;

    executor.update(start);

    assert(executor.state() ==
           RouteExecutorState::Turning);

    RouteExecutorFeedback finished;

    finished.turn_feedback.left_encoder = 100.0;
    finished.turn_feedback.right_encoder = -100.0;

    executor.update(finished);

    assert(executor.currentActionIndex() == 1);
    assert(executor.currentAction() ==
           Action::Stop);
    assert(executor.completed());
}

static void test_uturn_uses_back_turn()
{
    RouteExecutor executor(testConfig());

    Route route;

    route.actions = {
        Action::UTurn,
        Action::Stop
    };

    executor.loadRoute(route);
    executor.start();

    RouteExecutorFeedback start;

    executor.update(start);

    assert(executor.state() ==
           RouteExecutorState::Turning);

    RouteExecutorFeedback halfway;

    halfway.turn_feedback.left_encoder = 100.0;
    halfway.turn_feedback.right_encoder = -100.0;

    executor.update(halfway);

    assert(executor.state() ==
           RouteExecutorState::Turning);

    RouteExecutorFeedback finished;

    finished.turn_feedback.left_encoder = 200.0;
    finished.turn_feedback.right_encoder = -200.0;

    executor.update(finished);

    assert(executor.currentActionIndex() == 1);
    assert(executor.completed());
}

static void test_empty_route_faults()
{
    RouteExecutor executor(testConfig());

    Route route;

    executor.loadRoute(route);
    executor.start();

    assert(executor.state() ==
           RouteExecutorState::Fault);
}

static void test_reset()
{
    RouteExecutor executor(testConfig());

    Route route;

    route.actions = {
        Action::Right,
        Action::Stop
    };

    executor.loadRoute(route);
    executor.start();

    executor.reset();

    assert(executor.state() ==
           RouteExecutorState::Idle);

    assert(!executor.active());
    assert(!executor.completed());
    assert(executor.currentActionIndex() == 0);
}

int main()
{
    test_route_starts();
    test_forward_action_becomes_active();
    test_right_turn_starts_turn_controller();
    test_turn_advances_route();
    test_uturn_uses_back_turn();
    test_empty_route_faults();
    test_reset();

    return 0;
}