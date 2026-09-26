#include <cassert>

#include "perception/junction_detector.h"

int main()
{
    using namespace meshmerize;

    JunctionDetector detector;

    // --------------------------------------------------
    // Test 1: Normal centered straight line
    // --------------------------------------------------

    LineSensorReading straight;

    straight.values = {
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0
    };

    JunctionObservation result = detector.detect(straight);

    assert(!result.detected);
    assert(!result.left);
    assert(result.straight);
    assert(!result.right);

    // --------------------------------------------------
    // Test 2: Left junction
    // --------------------------------------------------

    LineSensorReading left_junction;

    left_junction.values = {
        1.0,
        1.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0
    };

    result = detector.detect(left_junction);

    assert(result.detected);
    assert(result.left);
    assert(result.straight);
    assert(!result.right);

    // --------------------------------------------------
    // Test 3: Right junction
    // --------------------------------------------------

    LineSensorReading right_junction;

    right_junction.values = {
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        1.0,
        1.0
    };

    result = detector.detect(right_junction);

    assert(result.detected);
    assert(!result.left);
    assert(result.straight);
    assert(result.right);

    // --------------------------------------------------
    // Test 4: Left + right junction
    // --------------------------------------------------

    LineSensorReading cross;

    cross.values = {
        1.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        1.0
    };

    result = detector.detect(cross);

    assert(result.detected);
    assert(result.left);
    assert(result.straight);
    assert(result.right);

    // --------------------------------------------------
    // Test 5: No line
    // --------------------------------------------------

    LineSensorReading no_line;

    no_line.values = {
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    result = detector.detect(no_line);

    assert(!result.detected);
    assert(!result.left);
    assert(!result.straight);
    assert(!result.right);

    // --------------------------------------------------
    // Test 6: Threshold behavior
    // --------------------------------------------------

    LineSensorReading threshold;

    threshold.values = {
        0.0,
        0.0,
        0.5,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0
    };

    result = detector.detect(threshold);

    assert(result.detected);
    assert(result.left);
    assert(result.straight);
    assert(!result.right);

    return 0;
}