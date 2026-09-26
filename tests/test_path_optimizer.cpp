#include <cassert>

#include "planning/path_optimizer.h"

using namespace meshmerize;

static void test_empty_route()
{
    PathOptimizer optimizer;

    Route route;

    const Route result = optimizer.optimize(route);

    assert(result.size() == 1);
    assert(result.actions[0] == Action::Stop);
}

static void test_adds_stop()
{
    PathOptimizer optimizer;

    Route route;

    route.actions = {
        Action::Forward,
        Action::Right,
        Action::Forward
    };

    const Route result = optimizer.optimize(route);

    assert(result.size() == 4);

    assert(result.actions[0] == Action::Forward);
    assert(result.actions[1] == Action::Right);
    assert(result.actions[2] == Action::Forward);
    assert(result.actions[3] == Action::Stop);
}

static void test_removes_internal_stop()
{
    PathOptimizer optimizer;

    Route route;

    route.actions = {
        Action::Forward,
        Action::Stop,
        Action::Right,
        Action::Stop,
        Action::Forward
    };

    const Route result = optimizer.optimize(route);

    assert(result.size() == 4);

    assert(result.actions[0] == Action::Forward);
    assert(result.actions[1] == Action::Right);
    assert(result.actions[2] == Action::Forward);
    assert(result.actions[3] == Action::Stop);
}

static void test_preserves_movement_order()
{
    PathOptimizer optimizer;

    Route route;

    route.actions = {
        Action::Left,
        Action::Forward,
        Action::Right,
        Action::UTurn
    };

    const Route result = optimizer.optimize(route);

    assert(result.size() == 5);

    assert(result.actions[0] == Action::Left);
    assert(result.actions[1] == Action::Forward);
    assert(result.actions[2] == Action::Right);
    assert(result.actions[3] == Action::UTurn);
    assert(result.actions[4] == Action::Stop);
}

static void test_does_not_modify_original()
{
    PathOptimizer optimizer;

    Route route;

    route.actions = {
        Action::Forward,
        Action::Stop,
        Action::Right
    };

    const Route result = optimizer.optimize(route);

    assert(route.size() == 3);

    assert(route.actions[0] == Action::Forward);
    assert(route.actions[1] == Action::Stop);
    assert(route.actions[2] == Action::Right);

    assert(result.size() == 3);
    assert(result.actions[0] == Action::Forward);
    assert(result.actions[1] == Action::Right);
    assert(result.actions[2] == Action::Stop);
}

int main()
{
    test_empty_route();
    test_adds_stop();
    test_removes_internal_stop();
    test_preserves_movement_order();
    test_does_not_modify_original();

    return 0;
}