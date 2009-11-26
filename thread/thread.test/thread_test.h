#pragma once

#include "thread_test_setup.h"
#include "../../thread.h"
#include "../../time.h"

using namespace Simone;

namespace SimoneTest {

BOOST_FIXTURE_TEST_SUITE(Activity_tests, ActivityManagerFixture);

BOOST_AUTO_TEST_CASE(Activity_test_basic_1) {
   Time start = Time(Clock::kMicrosecUniversal);
   test_reactor_1 = TestReactor::TestReactorNew(activity_1,
                                                TestReactor::kA,
                                                Activity::Config::kBlocking);
   TimeDelta diff = Time(Clock::kMicrosecUniversal) - start;
   cout << diff.totalSeconds() << endl;
   BOOST_CHECK(diff.totalSeconds() > 2);
   BOOST_CHECK(test_reactor_1->meaningOfLifeUniverseAndEverything() == 42);
}

BOOST_AUTO_TEST_SUITE_END();

} /* end of namespace SimoneTest */
