/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once
#include "../../test.h"
#include "../../math.h"
#include "../../utility.h"
#include "time_test_setup.h"

using namespace Simone;

namespace SimoneTest {

/*====================================================================================
===================================      Time      ===================================
====================================================================================*/


FIXTURE_TEST_SUITE(Time_tests, TimeFixture);

TEST_CASE(Time_test_basic_1) {
   CK(time_ == Time());
}

TEST_CASE(_2) {
   Time null1(Time::kNull);
   Time null2(Time::kNull);
   CK(null1 == null2);
}


SUITE_END();

/*====================================================================================
================================      TimeDelta      =================================
====================================================================================*/

FIXTURE_TEST_SUITE(TimeDelta_tests, TimeDeltaFixture);

TEST_CASE(TimeDelta_test_1) {
   long hrs = rand() % 100;
   long min = rand() % 60;
   long sec = rand() % 60;
   
   TimeDelta a = hours(hrs) + minutes(min) + seconds(sec);
   CK_EQUAL(a.totalSeconds(), totalSeconds(hrs, min, sec));
   CK_SMALL(a.totalSecondsDbl() - totalSecondsDbl(hrs, min, sec), Math::kEpsilon);
}

TEST_CASE(TimeDelta_test_2) {
   long hrs = rand() % 10000;
   long min = rand() % 10000;
   long sec = rand() % 10000;
   
   TimeDelta a = hours(hrs) + minutes(min) + seconds(sec);
   CK_EQUAL(a.totalSeconds(), totalSeconds(hrs, min, sec));
   CK_SMALL(a.totalSecondsDbl() - totalSecondsDbl(hrs, min, sec), Math::kEpsilon);
}

TEST_CASE(TimeDelta_test_2B) {
   long hrs = rand() % 10000;
   long min = rand() % 10000;
   long sec = rand() % 10000;
   
   TimeDelta a(hrs, min, sec);
   CK_EQUAL(a.totalSeconds(), totalSeconds(hrs, min, sec));
   CK_SMALL(a.totalSecondsDbl() - totalSecondsDbl(hrs, min, sec), Math::kEpsilon);
}

TEST_CASE(TimeDelta_test_2C) {
   long hrs = rand() % 10000;
   long min = rand() % 10000;
   long sec = rand() % 10000;
   long msec = rand() % 10000;
   
   TimeDelta a(hrs, min, sec);
   a += milliseconds(msec);
   CK_SMALL(a.totalSecondsDbl() - totalSecondsDbl(hrs,min,sec,msec), Math::kEpsilon);
}

TEST_CASE(TimeDelta_test_3) {
   long hrs  = rand() % 10000;
   long min  = rand() % 10000;
   long sec  = rand() % 10000;
   long msec = rand() % 10000;
   long Msec = rand() % 10000;
   long nsec = rand() % 10000;
   
   TimeDelta a = hours(hrs) +
                 minutes(min) +
                 seconds(sec);
   a += milliseconds(msec);
   a += microseconds(Msec);
   a += nanoseconds(nsec);
   CK_SMALL(a.totalSecondsDbl() - totalSecondsDbl(hrs, min, sec, msec, Msec, nsec),
            Math::kEpsilon);
}

TEST_CASE(TimeDelta_test_3b) {
   long hrs  = rand() % 10000;
   long min  = rand() % 10000;
   long sec  = rand() % 10000;
   long msec = rand() % 10000;
   long Msec = rand() % 10000;
   long nsec = rand() % 10000;
   
   CK_NE(boost::posix_time::time_res_traits::res_adjust(), 0);
   
   TimeDelta a = hours(hrs) +
                 minutes(min) +
                 seconds(sec) +
                 milliseconds(msec) +
                 microseconds(Msec) +
                 nanoseconds(nsec);
   CK_SMALL(a.totalSecondsDbl() - totalSecondsDbl(hrs, min, sec, msec, Msec, nsec),
            Math::kEpsilon);
}

SUITE_END();

} /* end of namespace SimoneTest */
