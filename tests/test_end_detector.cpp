#include <cassert>

#include "perception/end_detector.h"

int main()
{
    using namespace meshmerize;

    EndZoneDetector detector;

    // --------------------------------------------------
    // Test 1: Reset
    // --------------------------------------------------

    detector.reset();

        // --------------------------------------------------
    // Test 2: Establish normal line
    // --------------------------------------------------

    LineSensorReading line;

    line.values = {
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0
    };

    line.line_detected = true;

    assert(!detector.detect(line));

    // --------------------------------------------------
    // Test 3: First white reading
    // --------------------------------------------------

    LineSensorReading white;

    white.values = {
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    white.line_detected = false;

    assert(!detector.detect(white));

    // --------------------------------------------------
    // Test 4: Second white reading
    // --------------------------------------------------

    assert(!detector.detect(white));

    // --------------------------------------------------
    // Test 5: Third white reading
    // --------------------------------------------------

    assert(detector.detect(white));

    // --------------------------------------------------
    // Test 6: Reset clears detector state
    // --------------------------------------------------

    detector.reset();

    assert(!detector.detect(white));

        // --------------------------------------------------
    // Test 7: White area interrupted by a line
    // resets confirmation
    // --------------------------------------------------

    detector.reset();

    // Establish that the robot was following a line.
    assert(!detector.detect(line));

    // First white reading.
    assert(!detector.detect(white));

    // Second white reading.
    assert(!detector.detect(white));

    // Line interrupts the white sequence.
    assert(!detector.detect(line));

    // Confirmation must start again.
    assert(!detector.detect(white));
    assert(!detector.detect(white));

    // Third consecutive white reading after the
    // interruption confirms the end zone.
    assert(detector.detect(white));

    // --------------------------------------------------
    // Test 8: Sensor value above white threshold
    // means this is not a wide-white reading
    // --------------------------------------------------

    detector.reset();

    assert(!detector.detect(line));

    LineSensorReading almost_white;

    almost_white.values = {
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.3
    };

    almost_white.line_detected = false;

    assert(!detector.detect(almost_white));
    assert(!detector.detect(almost_white));
    assert(!detector.detect(almost_white));

    // --------------------------------------------------
    // Integration Test: Line -> End Zone
    // --------------------------------------------------

    detector.reset();

    // Robot is following the line.
    LineSensorReading normal_line;

    normal_line.values = {
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        0.0,
        0.0
    };

    normal_line.line_detected = true;

    assert(!detector.detect(normal_line));

    // Robot enters the white end zone.
    LineSensorReading end_zone;

    end_zone.values = {
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    end_zone.line_detected = false;

    // White sample 1.
    assert(!detector.detect(end_zone));

    // White sample 2.
    assert(!detector.detect(end_zone));

    // White sample 3.
    // End zone is confirmed.
    assert(detector.detect(end_zone));

    return 0;
}