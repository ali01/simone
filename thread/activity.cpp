#include "activity.h"
#include "activity_thread.h"
#include "activity_manager.h"
#include "../exception.h"

namespace Simone {

void Activity::runActivity() {
   assert(status() != Status::kRunning);
   statusIs(Status::kRunning);
   while ( ! notifiees_.empty()) {
      Notifiee *n = notifiees_.front();
      sleepUntil(n->nextTime());
      n->onStatus();
      notifiees_.popFront();
      if (status() == Status::kStopping) {
         lock lk(mutex_);
         status_ = Status::kReady;
         break;
      }
   }
}

void Activity::sleepUntil(const Time& _time) {
   if (_time <= Time(Clock::kMicrosecUniversal)) { return; }
   timed_lock lk(manager_->mutex_, absoluteTime(_time).ptime());
   while (timeDelta(_time) > milliseconds(0) && lk.owns_lock()) {
      statusIs(Status::kWaiting);
      manager_->time_delta_changed_.timed_wait(lk, absoluteTime(_time).ptime());
   }
   statusIs(Status::kRunning);
}

TimeDelta Activity::timeDelta(const Time& _time) const {
   return _time - manager_->currentTime();
}

Time Activity::absoluteTime(const Time& _time) const {
   TimeDelta delta = Time(Clock::kMicrosecUniversal) - manager_->currentTime();
   return _time + delta;
}

void
Activity::Notifiee::processExecutionMode(Config::ExecutionMode _e) {
   activity_->activity_thread_->executionModeIs(_e);
}

} //end namespace Simone
