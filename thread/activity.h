#pragma once

#include <string>
#include <functional>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

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

using std::string;
using std::binary_function;

// DEBUG
#warning remove
extern boost::recursive_mutex io_mutex;

namespace Simone {
/*     forward declaration     */ class ActivityManager;
class Activity : public PtrInterface<Activity>, private boost::noncopyable {
   typedef boost::recursive_mutex::scoped_lock scoped_lock_t;
   typedef boost::recursive_timed_mutex::scoped_lock timed_lock;
   friend class ActivityThread;
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const Activity> PtrConst;
   typedef Simone::Ptr<Activity> Ptr;
   
   struct Config {
      enum RunStatus { kRunning, kDone };
      enum SchedulingMode { kDefault, kAutomatic };
   };
   struct Status {
      enum RunStatus { kReady, kStopping, kWaiting, kRunning, kDone };
   };
   
   // factory constructor ============================================================
   static Ptr ActivityNew(const ActivityManager *const _m=NULL)
      { return new Activity(_m); }
   
   void runStatusIs(Config::RunStatus _s) {
      switch (_s) {
         case Config::kDone:
            if (runStatus() != Status::kDone) {
               runStatusIs(Status::kStopping);
            }
            break;
         case Config::kRunning:
            if (runStatus() == Status::kReady) { runActivity(); }
            else if (runStatus() == Status::kDone) {
               throw InvalidOperationException("activity execution complete");
            }
            break;
         default: throw AttributeNotSupportedException();
      }
   }
   
   Status::RunStatus runStatus() const {
      scoped_lock_t lk(mutex_);
      return run_status_;
   }
   
   const TimeDelta& autoTimeSpacing() const {
      scoped_lock_t lk(mutex_);
      return auto_time_spacing_;
   }
   
   void autoTimeSpacingIs(const TimeDelta& _td) {
      scoped_lock_t lk(mutex_);
      auto_time_spacing_ = _td;
   }
   
   /*=================================================================================
    * Activity task dispatch notifiee ==============================================*/
   class Task : public BaseNotifiee<Activity,Task,true> {
      friend class Activity;
   protected:
      Task() : scheduling_mode_(Activity::Config::kDefault) { test_value_ = 32; }
      
      Task(Time _next_time): 
                             next_time_(_next_time),
                             scheduling_mode_(Activity::Config::kDefault) {
                                test_value_ = 32;
                             }
                             
      ~Task() { scoped_lock_t lk(mutex_); }
      
      Time next_time_;
      Activity::Config::SchedulingMode scheduling_mode_;
   public:
      typedef Simone::Ptr<const Task> PtrConst;
      typedef Simone::Ptr<Task> Ptr;
      
      int test_value_;
      
      const Time& nextTime() const {
         scoped_lock_t lk(mutex_);
         return next_time_;
      }
      
      Activity::Config::SchedulingMode schedulingMode() const {
         scoped_lock_t lk(mutex_);
         return scheduling_mode_;
      }
      
      void schedulingModeIs(Activity::Config::SchedulingMode _m) {
         scoped_lock_t lk(mutex_);
         scheduling_mode_ = _m;
      }
      
      void notifierIs(const Activity::Ptr& _n) {
         scoped_lock_t lk(mutex_);
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
      scoped_lock_t lk(mutex_);
      if (_n->schedulingMode() == Config::kAutomatic) {
         _n->next_time_ = last_scheduled_time_ + autoTimeSpacing();
      } else if (_n->nextTime() == Time(Time::kNull)) {
         _n->next_time_ = Time(Clock::kMicrosecUniversal);
      }
      if (_n->nextTime() > last_scheduled_time_) {
         last_scheduled_time_ = _n->nextTime();
      }
      assert (_n->test_value_ == 32); // DEBUG
      run_queue_.push(_n);
      new_reactors_.notify_all();
   }
   
   void notifieeDel(Task *_n) const {
      scoped_lock_t lk(mutex_);
      Activity *me = const_cast<Activity *>(this);
      PriorityQueue<Task*,lt_TaskPtr,true>::iterator it = me->run_queue_.begin();
      for(; it != run_queue_.end(); ++it) { if (_n == *it) break; }
      if (it != run_queue_.end()) {
         me->run_queue_.elementDel(it);
      }
   }
   
   class Notifiee : public BaseNotifiee<Activity,Notifiee,true> {
   protected:
      Notifiee(Activity::Ptr _a) : test_value_(16) { // DEBUG
         notifierIs(_a);
      }
      
      ~Notifiee() { scoped_lock_t lk(mutex_); }
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
      scoped_lock_t lk(mutex_);
      Activity *me = const_cast<Activity*>(this);
      assert(_n->test_value_ == 16); // DEBUG
      // assert()
      me->notifiees_.elementIs(_n);
   }
   
   void notifieeDel(Notifiee *_n) const {
      scoped_lock_t lk(mutex_);
      Activity *me = const_cast<Activity*>(this);
      me->notifiees_.elementDel(_n);
   }
   
protected:
   Activity(const ActivityManager *const _m) :
                                   run_status_(Status::kReady),
                                   last_scheduled_time_(Clock::kMicrosecUniversal),
                                   auto_time_spacing_(0,0,0),
                                   manager_(_m) {}
   ~Activity() {
      scoped_lock_t lk(mutex_);
      runStatusIs(Status::kDone);
      threads_.join_all();
   }
private:
   // member functions ===============================================================
   void runActivity();
   
   void runStatusIs(Status::RunStatus _s) {
      scoped_lock_t lk(mutex_);
      run_status_ = _s;
      foreach(Notifiee *_n, notifiees_) { _n->onRunStatus(); }
   }
   
   void fireOnTaskCompleted(Task *task) const {
      scoped_lock_t lk(mutex_);
      Set<Notifiee*,true>::const_iterator it = notifiees_.begin();
      for (; it != notifiees_.end(); ++it) {
         Notifiee *n = *it;
         assert(n); // DEBUG
         assert(task);
         assert(n->test_value_ == 16);
         assert(task->test_value_ == 32);
         io_mutex.lock();
         cerr << this << " ~ fireOnTaskCompleted (start) ~ " << this_thread::id() << endl;
         cerr << "hey" << endl;
         cerr << n << endl;
         cerr << "hey" << endl;
         *n;
         io_mutex.unlock();
         
         n->onTaskCompleted(task);
         
         io_mutex.lock();
         cerr << this << " ~ fireOnTaskCompleted (end) ~   " << this_thread::id() << "\n" << endl;
         io_mutex.unlock();
         assert(n); // DEBUG
         assert(task);
         assert(n->test_value_ == 16);
         assert(task->test_value_ == 32);
      }
   }
   
   TimeDelta timeDelta(const Time& _time) const;
   
   Time absoluteTime(const Time& _time) const;
   
   void sleepUntil(const Time& _time);
   void waitForReactors() const;
   // data members ===================================================================
   Status::RunStatus run_status_;
   Time last_scheduled_time_;
   TimeDelta auto_time_spacing_;
   const ActivityManager *const manager_;
   
   Set<Notifiee*,true> notifiees_;
   PriorityQueue<Task*,lt_TaskPtr,true> run_queue_;
   
   mutable boost::recursive_mutex mutex_; // coarse granularity sync mutex
   mutable boost::condition_variable_any new_reactors_;
   
   boost::thread_group threads_;
};

} /* end of namespace Simone */
