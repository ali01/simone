/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once
#include "../../test.h"
#include "../../time.h"

using namespace Simone;

namespace SimoneTest {

struct TimeFixture {
   Time time_;
};

struct TimeDeltaFixture {
   TimeDeltaFixture() {
      srand(time(NULL));
   }
};

// utility ===========================================================================
double totalSecondsDbl(double hrs,
                       double min,
                       double sec,
                       double millisec=0,
                       double microsec=0,
                       double nanosec=0) {
   return 3600 * hrs +
          60 * min + sec +
          millisec / 1000 +
          microsec / 1000000 +
          nanosec  / 1000000000;
}

long long totalSeconds(long hrs, long min, long sec) {
   return 3600 * hrs + 60 * min + sec;
}

} /* end of namespace SimoneTest */
