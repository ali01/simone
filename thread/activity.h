#pragma once

#include <string>
#include <functional>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>

#include "../time.h"
#include "../notifiee.h"
#include "../exception.h"
#include "../utility.h"
#include "../numeric.h"
#include "../ordinal.h"
#include "../utility.h"

#include "this_thread.h"
#include "scoped_lock.h"
#include "recursive_mutex.h"
#include "concurrent_set.h"
#include "concurrent_pqueue.h"
#include "concurrent_ptr_interface.h"
#include "concurrent_collection_element.h"

using std::string;
using std::binary_function;

   // nextTime
   // currentTime
   // timeSinceStart

namespace Simone {
namespace thread {

/*     forward declaration     */ class ActivityManager;
class Activity : public ConcurrentPtrInterface<Activity>,
                 private boost::noncopyable {
   typedef boost::recursive_timed_mutex::scoped_lock timed_lock;
   friend class ActivityThread;
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const Activity> PtrConst;
   typedef Simone::Ptr<Activity> Ptr;
   
   struct config {
      enum RunStatus      { kRunning, kDone      };
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
   
   size_t numTasks() const {
      return run_queue_.size();
   }
   
   status::RunStatus runStatus() const {
      ScopedLock lk(this->mutex());
      return run_status_;
   }
   
   const TimeDelta& autoTimeSpacing() const {
      ScopedLock lk(this->mutex());
      return auto_time_spacing_;
   }
   
   void autoTimeSpacingIs(const TimeDelta& _td) {
      ScopedLock lk(this->mutex());
      auto_time_spacing_ = _td;
   }
   
   Time nextTime() const {
      ScopedLock lk(this->mutex());
      return run_queue_.front()->nextTime();
   }
   
   TimeDelta timeSinceStart() const;
   Time currentTime() const;
   
   /*=================================================================================
    * Activity task dispatch notifiee ==============================================*/
   class Task : public BaseNotifiee<Activity,Task>,
                public ConcurrentCollectionElement {
      friend class Activity;
   protected:
      Task() : scheduling_mode_(Activity::config::kDefault) {
         stronglyReferencingIs(false);
      }
      
      Task(Time _next_time): 
                             next_time_(_next_time),
                             scheduling_mode_(Activity::config::kDefault) {
         stronglyReferencingIs(false);
      }
                             
      virtual ~Task() { ScopedLock lk(this->mutex()); }
      
      Time next_time_;
      Activity::config::SchedulingMode scheduling_mode_;
   public:
      typedef Simone::Ptr<const Task> PtrConst;
      typedef Simone::Ptr<Task> Ptr;
      
      const Time& nextTime() const {
         ScopedLock lk(this->mutex());
         return next_time_;
      }
      
      Activity::config::SchedulingMode schedulingMode() const {
         ScopedLock lk(this->mutex());
         return scheduling_mode_;
      }
      
      void schedulingModeIs(Activity::config::SchedulingMode _m) {
         ScopedLock lk(this->mutex());
         scheduling_mode_ = _m;
      }
      
      void notifierIs(const Activity::Ptr& _n) {
         ScopedLock lk(this->mutex());
         BaseNotifiee<Activity,Task>::notifierIs(_n);
      }
      // notifications ---------------------------------------------------------------
      virtual void onRun() { ABORT(); }
   };
   
   struct lt_TaskPtr : public binary_function<Task::Ptr,Task::Ptr,bool> {
      bool operator()(Task::Ptr a, Task::Ptr b) const {
         return (a->nextTime() > b->nextTime());
      }
   };
   
   
   /*=================================================================================
    * Activity standard attribute notifiee =========================================*/
   void notifieeIs(Task::Ptr _n) {
      ScopedLock lk(this->mutex());
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
   
   void notifieeDel(Task::Ptr _n) const {
      ScopedLock lk(this->mutex());
      ScopedLock queue_lock(run_queue_.mutex());
      Activity *me = const_cast<Activity *>(this);
      ConcurrentPriorityQueue<Task::Ptr,lt_TaskPtr>::iterator it =
                                                               me->run_queue_.begin();
      for(; it != run_queue_.end(); ++it) { if (_n == *it) break; }
      if (it != run_queue_.end()) {
         me->run_queue_.elementDel(it);
      }
   }
   
   class Notifiee : public BaseNotifiee<Activity,Notifiee>,
                    public ConcurrentCollectionElement {
   protected:
      Notifiee(Activity::Ptr _a){
         notifierIs(_a);
         stronglyReferencingIs(false);
      }
      
      virtual ~Notifiee() { ScopedLock lk(this->mutex()); }
   public:
      typedef Simone::Ptr<const Notifiee> PtrConst;
      typedef Simone::Ptr<Notifiee> Ptr;
      // notifications ---------------------------------------------------------------
      virtual void onRunStatus()=0;
      virtual void onTaskCompleted(Activity::Task::Ptr) { ABORT(); } // todo: =0
   };
   
   // notification support -----------------------------------------------------------
   void notifieeIs(Notifiee::Ptr _n) const {
      ScopedLock lk(this->mutex());
      Activity *me = const_cast<Activity*>(this);
      me->notifiees_.elementIs(_n);
   }
   
   void notifieeDel(Notifiee::Ptr _n) const {
      ScopedLock lk(this->mutex());
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
      ScopedLock lk(this->mutex());
      runStatusIs(status::kDone);
      threads_.join_all();
   }
private:
   // member functions ===============================================================
   void runActivity();
   
   void runStatusIs(status::RunStatus _s) {
      ScopedLock lk(this->mutex());
      ScopedLock set_lk(notifiees_.mutex());
      run_status_ = _s;
      foreach(Notifiee::Ptr _n, notifiees_) {
         _n->onRunStatus();
      }
   }
   
   void fireOnTaskCompleted(Task::Ptr task) const {
      ScopedLock task_lock(task->mutex());
      ScopedLock notifiees_lock(notifiees_.mutex());
      
      foreach(Notifiee::Ptr n, notifiees_) {

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
   
   ConcurrentSet<Notifiee::Ptr> notifiees_;
   ConcurrentPriorityQueue<Task::Ptr,lt_TaskPtr> run_queue_;
   
   mutable boost::condition_variable_any new_reactors_;
   
   boost::thread_group threads_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
