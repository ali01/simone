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

   static Ptr TestReactorNew(Activity::Ptr _a, TestMode _m,
                             Activity::Config::ExecutionMode _e) {
      return new TestReactor(_a, _m, _e);
   }

   void onStatus();
   
   int meaningOfLifeUniverseAndEverything() const { return answer_; }
private:
   TestReactor(Activity::Ptr _a, TestMode _m,
               Activity::Config::ExecutionMode _e) : Activity::Notifiee(_a, _e),
                                                        test_mode_(_m),
                                                        answer_(0) {}
   TestMode test_mode_;
   int answer_;
};


struct ActivityManagerFixture {
   ActivityManagerFixture() : manager_(ActivityManager::ActivityManagerNew()),
                              activity_1(manager_->activityNew("a1"))
                              // activity_2(manager_->activityNew("a2")),
                              // activity_3(manager_->activityNew("a3")),
                              // activity_4(manager_->activityNew("a4"))
                              {}
   ActivityManager::Ptr manager_;
   Activity::Ptr activity_1;
   // Activity::Ptr activity_2;
   // Activity::Ptr activity_3;
   // Activity::Ptr activity_4;
   
   TestReactor::Ptr test_reactor_1;
   TestReactor::Ptr test_reactor_2;
   TestReactor::Ptr test_reactor_3;
   TestReactor::Ptr test_reactor_4;
};


} /* end of namespace thread_tester */
