/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#include "activity.h"
#include "activity_thread.h"
#include "activity_manager.h"
#include "../exception.h"

namespace Simone {
namespace thread {

void Activity::runActivity() {
   runStatusIs(status::kRunning);
   while (true) {
      waitForReactors();
      Task::Ptr n = run_queue_.front();
      sleepUntil(n->nextTime());
      n->onRun();
      fireOnTaskCompleted(n);
      run_queue_.pop();
      if (runStatus() == status::kStopping) {
         runStatusIs(status::kDone);
         break;
      }
   }
}

void Activity::waitForReactors() const {
   // ScopedLock lk(this->mutex());
   while (run_queue_.empty()) {
      assert(this->mutex().lockCount() == 0);
      this_thread::sleep(milliseconds(kSleepTime));
      // new_reactors_.wait(lk);
   }
   // TODO: write wrapper for ConditionVariable
}

TimeDelta Activity::timeSinceStart() const {
   ScopedLock lk(this->mutex());
   return manager_->timeSinceStart();
}

Time Activity::currentTime() const {
   ScopedLock lk(this->mutex());
   return manager_->currentTime();
}

void Activity::sleepUntil(const Time& _time) {
   if (_time <= currentTime()) { return; }
   runStatusIs(status::kWaiting);
   if (manager_) {
      // timed_lock lk(manager_->timed_mutex_, absoluteTime(_time).ptime());
      // while (timeDelta(_time) > milliseconds(0) && lk.owns_lock()) {
      //    manager_->time_delta_changed_.timed_wait(lk, absoluteTime(_time).ptime());
      // }
      while (true) {
         TimeDelta d = timeDelta(_time);
         // Time now = Time(Time::kNow); // COMMENT
         if (d < milliseconds(0)) break;
         assert(this->mutex().lockCount() == 0);
         this_thread::sleep(milliseconds(kSleepTime));
      }
   } else {
      assert(this->mutex().lockCount() == 0);
      this_thread::sleep(timeDelta(_time));
   }
   runStatusIs(status::kRunning);
}

TimeDelta Activity::timeDelta(const Time& _time) const {
   if (manager_) {
      Time current_time = manager_->currentTime();
      return _time - current_time;
   } else {
      return _time - Time(Time::kNow);
   }
}

Time Activity::absoluteTime(const Time& _time) const {
   if (manager_ == NULL) return _time;
   TimeDelta delta = Time(Clock::kMicrosecUniversal) - manager_->currentTime();
   return _time + delta;
}

} /* end of namespace thread */
} /* end of namespace Simone */
