#include "activity.h"
#include "activity_thread.h"
#include "activity_manager.h"
#include "../exception.h"

namespace Simone {
namespace thread {
   
#ifdef __DEBUG__
boost::recursive_mutex io_debug_mutex_;
#endif

void Activity::runActivity() {
   runStatusIs(status::kRunning);
   while (true) {
      waitForReactors();
      Task *n = run_queue_.front();
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
   ScopedLock lk(mutex_);
   while (run_queue_.empty()) {
      new_reactors_.wait(lk.boost_lock());
   }
}

void Activity::sleepUntil(const Time& _time) {
   if (_time <= Time(Clock::kMicrosecUniversal)) { return; }
   runStatusIs(status::kWaiting);
   if (manager_) {
      timed_lock lk(manager_->timed_mutex_, absoluteTime(_time).ptime());
      while (timeDelta(_time) > milliseconds(0) && lk.owns_lock()) {
         manager_->time_delta_changed_.timed_wait(lk, absoluteTime(_time).ptime());
      }
   } else { this_thread::sleep(timeDelta(_time)); }
   runStatusIs(status::kRunning);
}

TimeDelta Activity::timeDelta(const Time& _time) const {
   if (manager_) return _time - manager_->currentTime();
   else return _time - Time(Time::kNow);
}

Time Activity::absoluteTime(const Time& _time) const {
   if (manager_ == NULL) return _time;
   TimeDelta delta = Time(Clock::kMicrosecUniversal) - manager_->currentTime();
   return _time + delta;
}

} /* end of namespace thread */
} /* end of namespace Simone */
