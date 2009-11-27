#pragma once

#include "../../test.h"
#include "../../ptr.h"
#include "../../thread.h"

using namespace Simone;

namespace SimoneTest {

/*    forward declaration     */ class TestActivityTask;
class TestActivityReactor : public Activity::Notifiee {
public:
   typedef Simone::Ptr<const TestActivityReactor> PtrConst;
   typedef Simone::Ptr<TestActivityReactor> Ptr;
   
   static Ptr TestActivityReactorNew(Activity::Ptr _a, TestActivityTask *_t) {
      return new TestActivityReactor(_a, _t);
   }
   
   void onRunStatus() { /*lock lk(mutex_);*/ }
   void onTaskCompleted(Activity::Task *_task);
private:
   TestActivityReactor(Activity::Ptr _a, TestActivityTask *_t) :
                                          Activity::Notifiee(_a),
                                          task_(_t) { assert(task_); }
   TestActivityTask *task_;
};

class TestActivityTask : public Activity::Task {
public:
   typedef Simone::Ptr<const TestActivityTask> PtrConst;
   typedef Simone::Ptr<TestActivityTask> Ptr;
   
   enum TestMode { kA, kB, kC, kD };

   static Ptr TestActivityTaskNew(Activity::Ptr _a, TestMode _m) {
      return new TestActivityTask(_a, _m);
   }

   void onRun();
   
   int meaningOfLifeUniverseAndEverything() const {
      lock lk(mutex_);
      while ( ! answer_is_available_bool_) {
      // while (answer_ == 0) {
         answer_is_available_.wait(lk);
      }
      return answer_;
   }
   
   void answerIsAvailable() {
      lock(mutex_);
      answer_is_available_bool_ = true;
      answer_is_available_.notify_all();
   }
private:
   TestActivityTask(Activity::Ptr _a, TestMode _m) : 
                    reactor_(TestActivityReactor::TestActivityReactorNew(_a, this)),
                    test_mode_(_m),
                    answer_is_available_bool_(false),
                    answer_(0) { notifierIs(_a); }
   TestActivityReactor::Ptr reactor_;
   TestMode test_mode_;
   
   bool answer_is_available_bool_;
   int answer_;
   
   mutable boost::condition_variable_any answer_is_available_;
};

inline void TestActivityReactor::onTaskCompleted(Activity::Task *_task) {
   lock lk(mutex_);
   if (task_ == _task) {
      task_->answerIsAvailable();
   }
}


struct ActivityManagerFixture {
   ActivityManagerFixture() :
                              manager_(ActivityManager::ActivityManagerNew()),
                              activity_1(manager_->activityNew("a1")),
                              activity_2(manager_->activityNew("a2")),
                              activity_3(manager_->activityNew("a3")),
                              activity_4(manager_->activityNew("a4"))
                              {}
   ActivityManager::Ptr manager_;
   
   Activity::Ptr activity_1;
   Activity::Ptr activity_2;
   Activity::Ptr activity_3;
   Activity::Ptr activity_4;
   
   TestActivityTask::Ptr test_task_1;
   TestActivityTask::Ptr test_task_2;
   TestActivityTask::Ptr test_task_3;
   TestActivityTask::Ptr test_task_4;
};

} /* end of namespace thread_tester */
