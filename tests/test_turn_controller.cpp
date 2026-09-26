#include <cassert>

#include "control/turn_controller.h"

using namespace meshmerize;

static TurnConfig testConfig()
{
    TurnConfig config;

    config.target_ticks_90 = 100.0;
    config.tolerance_ticks = 3.0;
    config.turn_speed = 0.5;

    return config;
}

static void test_left_turn()
{
    TurnController controller(testConfig());

    TurnFeedback start;
    start.left_encoder = 100.0;
    start.right_encoder = 200.0;

    controller.start(
        RelativeDirection::Left,
        start
    );

    assert(controller.active());

    TurnCommand command =
        controller.update(start);

    assert(command.left_motor < 0.0);
    assert(command.right_motor > 0.0);
    assert(!command.complete);
}

static void test_right_turn()
{
    TurnController controller(testConfig());

    TurnFeedback start;
    start.left_encoder = 100.0;
    start.right_encoder = 200.0;

    controller.start(
        RelativeDirection::Right,
        start
    );

    TurnCommand command =
        controller.update(start);

    assert(command.left_motor > 0.0);
    assert(command.right_motor < 0.0);
    assert(!command.complete);
}

static void test_turn_completes()
{
    TurnController controller(testConfig());

    TurnFeedback start;
    start.left_encoder = 0.0;
    start.right_encoder = 0.0;

    controller.start(
        RelativeDirection::Right,
        start
    );

    TurnFeedback feedback;
    feedback.left_encoder = 100.0;
    feedback.right_encoder = -100.0;

    TurnCommand command =
        controller.update(feedback);

    assert(command.complete);
    assert(command.left_motor == 0.0);
    assert(command.right_motor == 0.0);
    assert(!controller.active());
}

static void test_turn_not_complete_before_target()
{
    TurnController controller(testConfig());

    TurnFeedback start;
    start.left_encoder = 0.0;
    start.right_encoder = 0.0;

    controller.start(
        RelativeDirection::Right,
        start
    );

    TurnFeedback feedback;
    feedback.left_encoder = 40.0;
    feedback.right_encoder = -40.0;

    TurnCommand command =
        controller.update(feedback);

    assert(!command.complete);
    assert(controller.active());
}

static void test_straight_completes_immediately()
{
    TurnController controller(testConfig());

    TurnFeedback start;

    controller.start(
        RelativeDirection::Straight,
        start
    );

    TurnCommand command =
        controller.update(start);

    assert(command.complete);
    assert(!controller.active());
}

static void test_back_requires_more_rotation()
{
    TurnController controller(testConfig());

    TurnFeedback start;

    controller.start(
        RelativeDirection::Back,
        start
    );

    TurnFeedback halfway;
    halfway.left_encoder = 100.0;
    halfway.right_encoder = -100.0;

    TurnCommand command =
        controller.update(halfway);

    assert(!command.complete);
    assert(controller.active());

    TurnFeedback finished;
    finished.left_encoder = 200.0;
    finished.right_encoder = -200.0;

    command = controller.update(finished);

    assert(command.complete);
    assert(!controller.active());
}

static void test_reset()
{
    TurnController controller(testConfig());

    TurnFeedback start;

    controller.start(
        RelativeDirection::Right,
        start
    );

    assert(controller.active());

    controller.reset();

    assert(!controller.active());
}

int main()
{
    test_left_turn();
    test_right_turn();
    test_turn_completes();
    test_turn_not_complete_before_target();
    test_straight_completes_immediately();
    test_back_requires_more_rotation();
    test_reset();

    return 0;
}