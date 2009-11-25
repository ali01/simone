#pragma once

#include "../../ptr.h"
#include "../../thread.h"

using namespace Simone;

namespace SimoneTest {

class TestReactor : public Activity::Notifiee {
public:
   typedef Simone::Ptr<const TestReactor> PtrConst;
   typedef Simone::Ptr<TestReactor> Ptr;
   
   enum TestMode { kA, kB, kC, kD };

   static Ptr TestReactorNew(Activity::Ptr _a, TestMode _m) {
      return new TestReactor(_a, _m);
   }

   void onStatus() {
      switch (test_mode_) {
         case kA:
            answer_ = 42;
            break;
         case kB:
         case kC:
         case kD:
         default: return;
      }
   }
   
   int meaningOfLifeUniverseAndEverything() const { return answer_; }
private:
   TestReactor(Activity::Ptr _a, TestMode _m) : Activity::Notifiee(_a),
                                                        test_mode_(_m),
                                                        answer_(0) {}
   TestMode test_mode_;
   int answer_;
};

struct ActivityManagerFixture {
   ActivityManagerFixture() : manager_(ActivityManager::ActivityManagerNew()),
                              activity_1(manager_->activityNew("a1")),
                              activity_2(manager_->activityNew("a2")),
                              activity_3(manager_->activityNew("a3")),
                              activity_4(manager_->activityNew("a4")) {}
   ActivityManager::Ptr manager_;
   Activity::Ptr activity_1;
   Activity::Ptr activity_2;
   Activity::Ptr activity_3;
   Activity::Ptr activity_4;
   
   TestReactor::Ptr test_reactor_1;
   TestReactor::Ptr test_reactor_2;
   TestReactor::Ptr test_reactor_3;
   TestReactor::Ptr test_reactor_4;
};

BOOST_FIXTURE_TEST_SUITE(Activity_tests, ActivityManagerFixture);

BOOST_AUTO_TEST_CASE(Activity_test_basic_1) {
   test_reactor_1 = TestReactor::TestReactorNew(activity_1, TestReactor::kA);
   BOOST_CHECK(test_reactor_1->meaningOfLifeUniverseAndEverything() == 42);
}

BOOST_AUTO_TEST_SUITE_END();

} /* end of namespace thread_tester */
