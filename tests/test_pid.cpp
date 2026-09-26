#include <cassert>
#include <cmath>

#include "control/pid.h"

namespace {

void assert_near(double actual, double expected, double tolerance = 1e-9)
{
    assert(std::fabs(actual - expected) <= tolerance);
}

}

int main()
{
    using namespace meshmerize;

    // --------------------------------------------------
    // Test 1: Proportional control
    // --------------------------------------------------

    PIDConfig config;
    config.kp = 2.0;
    config.ki = 0.0;
    config.kd = 0.0;

    PIDController pid(config);

    double output = pid.update(3.0, 0.1);

    // 2.0 * 3.0 = 6.0
    assert_near(output, 6.0);

    // --------------------------------------------------
    // Test 2: Integral control
    // --------------------------------------------------

    pid.reset();

    config.kp = 0.0;
    config.ki = 2.0;
    config.kd = 0.0;

    PIDController integral_pid(config);

    integral_pid.update(1.0, 1.0);

    output = integral_pid.update(1.0, 1.0);

    // Integral after two seconds:
    // 1*1 + 1*1 = 2
    //
    // Output = Ki * integral
    //        = 2 * 2
    //        = 4
    assert_near(output, 4.0);

    // --------------------------------------------------
    // Test 3: Derivative control
    // --------------------------------------------------

    config.kp = 0.0;
    config.ki = 0.0;
    config.kd = 2.0;

    PIDController derivative_pid(config);

    derivative_pid.update(1.0, 1.0);

    output = derivative_pid.update(3.0, 1.0);

    // derivative = (3 - 1) / 1 = 2
    //
    // output = 2 * 2 = 4
    assert_near(output, 4.0);

    // --------------------------------------------------
    // Test 4: Output upper clamp
    // --------------------------------------------------

    config.kp = 100.0;
    config.ki = 0.0;
    config.kd = 0.0;
    config.output_min = -10.0;
    config.output_max = 10.0;

    PIDController clamp_pid(config);

    output = clamp_pid.update(1.0, 0.1);

    assert_near(output, 10.0);

    // --------------------------------------------------
    // Test 5: Output lower clamp
    // --------------------------------------------------

    output = clamp_pid.update(-1.0, 0.1);

    assert_near(output, -10.0);

    // --------------------------------------------------
    // Test 6: Reset
    // --------------------------------------------------

    PIDConfig reset_config;
    reset_config.kp = 0.0;
    reset_config.ki = 1.0;
    reset_config.kd = 0.0;

    PIDController reset_pid(reset_config);

    reset_pid.update(5.0, 1.0);

    reset_pid.reset();

    output = reset_pid.update(1.0, 1.0);

    // After reset:
    // integral = 1
    // output = 1
    assert_near(output, 1.0);

    return 0;
}