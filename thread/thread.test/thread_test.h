#pragma once
#include "../../test.h"
#include "../../thread.h"
#include "../../time.h"

#include "thread_test_setup.h"

using namespace Simone;

namespace SimoneTest {

FIXTURE_TEST_SUITE(a1, ActivityManagerFixture);

TEST_CASE(_1) {
   test_task_1 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kA);
   CK_EQUAL(test_task_1->meaningOfLifeUniverseAndEverything(), 42);
}

TEST_CASE(_2) { // sequential tasks
   test_task_1 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kA);
   test_task_2 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kB);
   test_task_3 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kC);
   test_task_4 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kD);
   CK_EQUAL(test_task_1->meaningOfLifeUniverseAndEverything(), 42);
   CK_EQUAL(test_task_2->meaningOfLifeUniverseAndEverything(), 2737);
   CK_EQUAL(test_task_3->meaningOfLifeUniverseAndEverything(), 38193);
   CK_EQUAL(test_task_4->meaningOfLifeUniverseAndEverything(), 29218);
}

TEST_CASE(_3) { // sequential tasks 2
   test_task_1 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kA);
   CK_EQUAL(test_task_1->meaningOfLifeUniverseAndEverything(), 42);
   test_task_2 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kB);
   CK_EQUAL(test_task_2->meaningOfLifeUniverseAndEverything(), 2737);
   test_task_3 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kC);
   CK_EQUAL(test_task_3->meaningOfLifeUniverseAndEverything(), 38193);
   test_task_4 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kD);
   CK_EQUAL(test_task_4->meaningOfLifeUniverseAndEverything(), 29218);
}

TEST_CASE(_4) { // different activities
   test_task_1 = TestActivityTask::TestActivityTaskNew(activity_1,
                                                       TestActivityTask::kA);
   test_task_2 = TestActivityTask::TestActivityTaskNew(activity_2,
                                                       TestActivityTask::kB);
   test_task_3 = TestActivityTask::TestActivityTaskNew(activity_3,
                                                       TestActivityTask::kC);
   test_task_4 = TestActivityTask::TestActivityTaskNew(activity_4,
                                                       TestActivityTask::kD);
   CK_EQUAL(test_task_1->meaningOfLifeUniverseAndEverything(), 42);
   CK_EQUAL(test_task_2->meaningOfLifeUniverseAndEverything(), 2737);
   CK_EQUAL(test_task_3->meaningOfLifeUniverseAndEverything(), 38193);
   CK_EQUAL(test_task_4->meaningOfLifeUniverseAndEverything(), 29218);
}

SUITE_END();

} /* end of namespace SimoneTest */
