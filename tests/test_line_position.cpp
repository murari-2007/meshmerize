#include <cassert>
#include <cmath>

#include "perception/line_position.h"

namespace {

void assert_near(double actual, double expected)
{
    assert(std::abs(actual - expected) < 1e-9);
}

}

int main()
{
    using namespace meshmerize;

    LinePositionCalculator calculator;

    // --------------------------------------------------------
    // Centered line
    // --------------------------------------------------------

    LineSensorReading centered;

    centered.values = {
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0
    };

    LinePosition center_result =
        calculator.calculate(centered);

    assert(center_result.line_detected);
    assert_near(center_result.position, 0.0);

    // --------------------------------------------------------
    // Line to the left
    // --------------------------------------------------------

    LineSensorReading left;

    left.values = {
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    LinePosition left_result =
        calculator.calculate(left);

    assert(left_result.line_detected);
    assert_near(left_result.position, -1.0);

    // --------------------------------------------------------
    // Line to the right
    // --------------------------------------------------------

    LineSensorReading right;

    right.values = {
        0.0,
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0
    };

    LinePosition right_result =
        calculator.calculate(right);

    assert(right_result.line_detected);
    assert_near(right_result.position, 1.0);

    // --------------------------------------------------------
    // Fractional position
    // --------------------------------------------------------

    LineSensorReading fractional;

    fractional.values = {
        0.0,
        0.0,
        0.25,
        1.0,
        0.75,
        0.0,
        0.0,
        0.0
    };

    LinePosition fractional_result =
        calculator.calculate(fractional);

    assert(fractional_result.line_detected);

    /*
     * Expected:
     *
     * (-1.5 * 0.25)
     * +(-0.5 * 1.0)
     * +( 0.5 * 0.75)
     * ----------------
     *      2.0
     *
     * = -0.1875
     */

    assert_near(
    fractional_result.position,
    -0.25
);

    // --------------------------------------------------------
    // Line completely lost
    // --------------------------------------------------------

    LineSensorReading lost;

    lost.values = {
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    LinePosition lost_result =
        calculator.calculate(lost);

    assert(!lost_result.line_detected);

    /*
     * Position is 0 for the numeric output, BUT
     * line_detected is false.
     *
     * Therefore downstream code must NOT interpret this
     * as "perfectly centered".
     */
    assert_near(lost_result.position, 0.0);

    return 0;
}