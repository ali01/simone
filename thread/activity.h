#pragma once

#include <string>
#include <functional>

#include <boost/thread/thread.hpp>

#include "../time.h"
#include "../ptr.h"
#include "../set.h"
#include "../pqueue.h"
#include "../notifiee.h"
#include "../exception.h"
#include "../utility.h"
#include "../numeric.h"
#include "../ordinal.h"
#include "../utility.h"

#include "this_thread.h"
#include "recursive_mutex.h"
#include "scoped_lock.h"

using std::string;
using std::binary_function;

namespace Simone {
namespace thread {

#ifdef __DEBUG__
extern boost::recursive_mutex io_debug_mutex_;
#endif

/*     forward declaration     */ class ActivityManager;
class Activity : public PtrInterface<Activity>, private boost::noncopyable {
   typedef boost::recursive_timed_mutex::scoped_lock timed_lock;
   friend class ActivityThread;
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const Activity> PtrConst;
   typedef Simone::Ptr<Activity> Ptr;
   
   struct config {
      enum RunStatus { kRunning, kDone };
      enum SchedulingMode { kDefault, kAutomatic };
   };
   struct status {
      enum RunStatus { kReady, kStopping, kWaiting, kRunning, kDone };
   };
   
   // factory constructor ============================================================
   static Ptr ActivityNew(const ActivityManager *const _m=NULL)
      { return new Activity(_m); }
   
   void runStatusIs(config::RunStatus _s) {
      switch (_s) {
         case config::kDone:
            if (runStatus() != status::kDone) {
               runStatusIs(status::kStopping);
            }
            break;
         case config::kRunning:
            if (runStatus() == status::kReady) { runActivity(); }
            else if (runStatus() == status::kDone) {
               throw InvalidOperationException("activity execution complete");
            }
            break;
         default: throw AttributeNotSupportedException();
      }
   }
   
   status::RunStatus runStatus() const {
      ScopedLock lk(mutex_);
      return run_status_;
   }
   
   const TimeDelta& autoTimeSpacing() const {
      ScopedLock lk(mutex_);
      return auto_time_spacing_;
   }
   
   void autoTimeSpacingIs(const TimeDelta& _td) {
      ScopedLock lk(mutex_);
      auto_time_spacing_ = _td;
   }
   
   /*=================================================================================
    * Activity task dispatch notifiee ==============================================*/
   class Task : public BaseNotifiee<Activity,Task,true> {
      friend class Activity;
   protected:
      Task() : scheduling_mode_(Activity::config::kDefault) { test_value_ = 32; }
      
      Task(Time _next_time): 
                             next_time_(_next_time),
                             scheduling_mode_(Activity::config::kDefault) {
                                test_value_ = 32;
                             }
                             
      virtual ~Task() { ScopedLock lk(mutex_); }
      
      Time next_time_;
      Activity::config::SchedulingMode scheduling_mode_;
   public:
      typedef Simone::Ptr<const Task> PtrConst;
      typedef Simone::Ptr<Task> Ptr;
      
      int test_value_;
      
      const Time& nextTime() const {
         ScopedLock lk(mutex_);
         return next_time_;
      }
      
      Activity::config::SchedulingMode schedulingMode() const {
         ScopedLock lk(mutex_);
         return scheduling_mode_;
      }
      
      void schedulingModeIs(Activity::config::SchedulingMode _m) {
         ScopedLock lk(mutex_);
         scheduling_mode_ = _m;
      }
      
      void notifierIs(const Activity::Ptr& _n) {
         ScopedLock lk(mutex_);
         BaseNotifiee<Activity,Task,true>::notifierIs(_n);
      }
      // notifications ---------------------------------------------------------------
      virtual void onRun() {}
   };
   
   struct lt_TaskPtr : public binary_function<Task*,Task*,bool> {
      bool operator()(Task *a, Task *b) const {
         return (a->nextTime() > b->nextTime());
      }
   };
   
   
   /*=================================================================================
    * Activity standard attribute notifiee =========================================*/
   void notifieeIs(Task *_n) {
      ScopedLock lk(mutex_);
      if (_n->schedulingMode() == config::kAutomatic) {
         _n->next_time_ = last_scheduled_time_ + autoTimeSpacing();
      } else if (_n->nextTime() == Time(Time::kNull)) {
         _n->next_time_ = Time(Clock::kMicrosecUniversal);
      }
      if (_n->nextTime() > last_scheduled_time_) {
         last_scheduled_time_ = _n->nextTime();
      }
      run_queue_.push(_n);
      new_reactors_.notify_all();
   }
   
   void notifieeDel(Task *_n) const {
      ScopedLock lk(mutex_);
      ScopedLock queue_mutex(run_queue_.mutex());
      Activity *me = const_cast<Activity *>(this);
      PriorityQueue<Task*,lt_TaskPtr,true>::iterator it = me->run_queue_.begin();
      for(; it != run_queue_.end(); ++it) { if (_n == *it) break; }
      if (it != run_queue_.end()) {
         me->run_queue_.elementDel(it);
      }
   }
   
   class Notifiee : public BaseNotifiee<Activity,Notifiee,true> {
   protected:
      Notifiee(Activity::Ptr _a){
         notifierIs(_a);
      }
      
      virtual ~Notifiee() { ScopedLock lk(mutex_); }
   public:
      int test_value_;
      typedef Simone::Ptr<const Notifiee> PtrConst;
      typedef Simone::Ptr<Notifiee> Ptr;
      // notifications ---------------------------------------------------------------
      virtual void onRunStatus() { ABORT(); }
      virtual void onTaskCompleted(Activity::Task *) { ABORT(); }
   };
   
   // notification support -----------------------------------------------------------
   void notifieeIs(Notifiee *_n) const {
      ScopedLock lk(mutex_);
      Activity *me = const_cast<Activity*>(this);
      me->notifiees_.elementIs(_n);
   }
   
   void notifieeDel(Notifiee *_n) const {
      ScopedLock lk(mutex_);
      Activity *me = const_cast<Activity*>(this);
      me->notifiees_.elementDel(_n);
   }
   
protected:
   Activity(const ActivityManager *const _m) :
                                   run_status_(status::kReady),
                                   last_scheduled_time_(Clock::kMicrosecUniversal),
                                   auto_time_spacing_(0,0,0),
                                   manager_(_m) {}
   virtual ~Activity() {
      ScopedLock lk(mutex_);
      runStatusIs(status::kDone);
      threads_.join_all();
   }
private:
   // member functions ===============================================================
   void runActivity();
   
   void runStatusIs(status::RunStatus _s) {
      ScopedLock lk(mutex_);
      ScopedLock set_lk(notifiees_.mutex());
      run_status_ = _s;
      foreach(Notifiee *_n, notifiees_) { _n->onRunStatus(); }
   }
   
   void fireOnTaskCompleted(Task *task) const {
      ScopedLock lk(mutex_);
      ScopedLock set_lk(notifiees_.mutex());
      foreach(Notifiee *n, notifiees_) {

         // assert(n); // COMMENT
         // assert(task);
         // assert(n->test_value_ == 16);
         // assert(task->test_value_ == 32);
         // io_debug_mutex_.lock();
         // cerr << this << " ~ fireOnTaskCompleted (start) ~ " << this_thread::id()
         //                                                                   << endl;
         // cerr << "hey" << endl;
         // cerr << n << endl;
         // cerr << "hey" << endl;
         // *n;
         // io_debug_mutex_.unlock();
         
         n->onTaskCompleted(task);
         
         // io_debug_mutex_.lock();
         // cerr << this << " ~ fireOnTaskCompleted (end) ~   " << this_thread::id()
         //                                                          << "\n" << endl;
         // io_debug_mutex_.unlock();
         // assert(n);
         // assert(task);
         // assert(n->test_value_ == 16);
         // assert(task->test_value_ == 32);
      }
   }
   
   TimeDelta timeDelta(const Time& _time) const;
   
   Time absoluteTime(const Time& _time) const;
   
   void sleepUntil(const Time& _time);
   void waitForReactors() const;
   // data members ===================================================================
   status::RunStatus run_status_;
   Time last_scheduled_time_;
   TimeDelta auto_time_spacing_;
   const ActivityManager *const manager_;
   
   Set<Notifiee*,true> notifiees_;
   PriorityQueue<Task*,lt_TaskPtr,true> run_queue_;
   
   mutable RecursiveMutex mutex_; // coarse granularity sync mutex
   mutable boost::condition_variable_any new_reactors_;
   
   boost::thread_group threads_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
