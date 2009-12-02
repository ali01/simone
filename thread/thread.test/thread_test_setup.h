#pragma once
#include "../thread.h"
#include "../../test.h"
#include "../../ptr_interface.h"

using namespace Simone;
using namespace Simone::thread;

namespace SimoneTest {

/*    forward declaration     */ class TestActivityTask;
class TestActivityReactor : public Activity::Notifiee {
public:
   typedef Simone::Ptr<const TestActivityReactor> PtrConst;
   typedef Simone::Ptr<TestActivityReactor> Ptr;
   
   static Ptr TestActivityReactorNew(Activity::Ptr _a,
                                     Simone::Ptr<TestActivityTask> _t) {
      return new TestActivityReactor(_a, _t);
   }
   
   void onRunStatus() { /*ScopedLock lk(this->mutex());*/ }
   void onTaskCompleted(Activity::Task::Ptr);
private:
   TestActivityReactor(Activity::Ptr _a, Simone::Ptr<TestActivityTask> _t) :
                                          Activity::Notifiee(_a),
                                          task_(_t) {
      assert(task_);
      notifierIs(_a);
   }
   
   ~TestActivityReactor() {
      ScopedLock lk(this->mutex());
      // io_mutex.lock();
      // cout << this << "~TestActivityReactor" << this_thread::id() << endl;
      // io_mutex.unlock();
   }
   
   Simone::Ptr<TestActivityTask> task_;
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
      // io_mutex.lock(); //COMMENT
      // cout << this << " ~ meaningOfLife (start) ~       " << this_thread::id() << endl;
      // io_mutex.unlock();
      
      // boost::recursive_mutex::scoped_lock lk(this->mutex());
      bool ready;
      {
         ScopedLock lk(this->mutex());
         ready = answer_is_available_bool_;
      }
      while ( ! ready) {
         assert(this->mutex().lockCount() == 0);
         this_thread::sleep(milliseconds(Activity::kSleepTime));
         {
            ScopedLock lk(this->mutex());
            ready = answer_is_available_bool_;
         }
         // answer_is_available_.wait(lk); // TODO: implement ConditionVars
      }
      
      // io_mutex.lock();
      // cout << this << " ~ meaningOfLife (end) ~         " << this_thread::id() << endl;
      // io_mutex.unlock();
      return answer_;
   }
   
   void answerIsAvailable() {
      ScopedLock lk(this->mutex());
      // io_mutex.lock();
      // cout << this << " ~ answerIsAvailable (start) ~   " << this_thread::id() << endl;
      // io_mutex.unlock();
      
      answer_is_available_bool_ = true;
      // answer_is_available_.notify_all();
      
      // assert(test_value_ == 32); // COMMENT
      // io_mutex.lock();
      // cout << this << " ~ answerIsAvailable (end) ~     " << this_thread::id() << endl;
      // io_mutex.unlock();
   }
private:
   TestActivityTask(Activity::Ptr _a, TestMode _m) : 
                    reactor_(TestActivityReactor::TestActivityReactorNew(_a, this)),
                    test_mode_(_m),
                    answer_is_available_bool_(false),
                    answer_(0) {
      assert(_a);
      notifierIs(_a);
   }
   
   ~TestActivityTask() {
      ScopedLock lk(this->mutex());
   }
   
   TestActivityReactor::Ptr reactor_;
   TestMode test_mode_;
   
   bool answer_is_available_bool_;
   int answer_;
   
   mutable boost::condition_variable_any answer_is_available_;
};

inline void TestActivityReactor::onTaskCompleted(Activity::Task::Ptr _task) {
   ScopedLock lk(this->mutex());

   // io_mutex.lock(); // COMMENT
   // cout << this << " ~ onTaskCompleted (start) ~     " << this_thread::id() << endl;
   // io_mutex.unlock();
   // assert(_task);
   // assert(_task->test_value_ == 32);

   if (task_.ptr() == _task.ptr()) {
      task_->answerIsAvailable();
   }

   // assert(_task);
   // assert(_task->test_value_ == 32); // COMMENT
   // io_mutex.lock();
   // cout << this << " ~ onTaskCompleted (end) ~       " << this_thread::id() << endl;
   // io_mutex.unlock();
}


struct ActivityManagerFixture {
   ActivityManagerFixture() :
                              manager_(ActivityManager::ActivityManagerNew()),
                              activity_1(manager_->activityNew("a1"))
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
