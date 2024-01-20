#include "minunit.h"

#include "hsv_test.h"

#include <stdio.h>

char *allTests(void) {
  // Begin the test suite
  muSuiteStart();

  // hsv_test
  muRunTest(test_hsv_to_and_from_rainbow);
  // muRunTest(test_hsv_size);

  // muRunTest(test_hsv_big_boi);
  // muRunTest(test_hsv_to_bytes);
  // muRunTest(test_hsv_compare);
  // muRunTest(test_hsv_to_rgb_colors);
  // muRunTest(test_hsv_near_yellow);
  // muRunTest(test_hsv_yellow);
  // muRunTest(test_hsv_interpolate_rgb);
  // muRunTest(test_hsv_simple);
  // muRunTest(test_hsv_interpolate);
  return NULL;
}

RUN_TESTS(allTests)
