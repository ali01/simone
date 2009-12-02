#pragma once

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>

#include "../ptr_interface.h"

#include "concurrent_map.h"
#include "concurrent_deque.h"
#include "activity_thread.h"
#include "activity.h"

namespace Simone {
namespace thread {
// todo: unspecialize
class ActivityManager : public ConcurrentMap<string,Activity::Ptr> {
   friend class Activity;
   typedef boost::recursive_timed_mutex::scoped_lock scoped_timed_lock;
public:
   typedef Simone::Ptr<const ActivityManager> PtrConst;
   typedef Simone::Ptr<ActivityManager> Ptr;

   static Ptr ActivityManagerNew();
   Activity::Ptr activity(const string &name) const;
   // instantiating attributes -------------------------------------------------------
   Activity::Ptr activityNew(const string &name);
   void          activityDel(const string &name);   
   // attributes =====================================================================
   
   void timeSinceStartIs(TimeDelta& _delta) {
      ScopedLock lk(this->mutex());
      timeDeltaIs((base_time_ + _delta) - Time(Clock::kMicrosecUniversal));
   }
   
   TimeDelta timeSinceStart() const {
      ScopedLock lk(this->mutex());
      return (Time(Clock::kMicrosecUniversal) - base_time_) + time_delta_;
   }
   
   Time currentTime() const {
      return Time(Clock::kMicrosecUniversal) + time_delta_;
   }
   
   void currentTimeIs(Time &_time) {
      ScopedLock lk(this->mutex());
      timeDeltaIs(_time - Time(Clock::kMicrosecUniversal));
   }
   
   TimeDelta timeDelta() const {
      ScopedLock lk(this->mutex());
      return time_delta_;
   }
   
   void timeDeltaIs(const TimeDelta& _delta) {
      {
         ScopedLock lk(this->mutex());
         time_delta_ = _delta;
      }
      boost::recursive_timed_mutex::scoped_lock lk(timed_mutex_);
      time_delta_changed_.notify_all();
   }
   
   void timeDeltaInc(const TimeDelta& _delta) {
      {
         ScopedLock lk(this->mutex());
         time_delta_ += _delta;
      }
      boost::recursive_timed_mutex::scoped_lock lk(timed_mutex_);
      time_delta_changed_.notify_all();
   }
   
   void timeDeltaDec(const TimeDelta& _delta) {
      {
         ScopedLock lk(this->mutex());
         time_delta_ -= _delta;
      }
      boost::recursive_timed_mutex::scoped_lock lk(timed_mutex_);
      time_delta_changed_.notify_all();
   }
private:
   ActivityManager() : base_time_(Clock::kMicrosecUniversal), time_delta_(0,0,0) {}
   virtual ~ActivityManager() { ScopedLock lk(this->mutex()); }
   
   class ActivityReactor : public Activity::Notifiee {
   public:
      typedef Simone::Ptr<const ActivityReactor> PtrConst;
      typedef Simone::Ptr<ActivityReactor> Ptr;
      
      static Ptr ActivityReactorNew(Activity::Ptr _notifier, ActivityManager *_owner)
                                    { return new ActivityReactor(_notifier, _owner); }
      // overridden notifications ----------------------------------------------------
      void onRunStatus() {}
      void onTaskCompleted(Activity::Task::Ptr) {}
   private:
      ActivityReactor(Activity::Ptr _notifier, ActivityManager *_owner) : 
                      Activity::Notifiee(_notifier), owner_(_owner){}
      ActivityManager *owner_;
   };
   
   
   // data members ===================================================================
   Time base_time_;
   TimeDelta time_delta_;
   ConcurrentDeque<ActivityThread::Ptr> threads_;
   ConcurrentMap<string,ActivityReactor::Ptr> activity_reactors_;
   
   mutable boost::condition_variable_any time_delta_changed_;
   mutable boost::recursive_timed_mutex timed_mutex_;
};

inline Activity::Ptr ActivityManager::activity(const string &name) const {
   const_iterator iter = this->element(name);
   if (iter != this->end()) {
      return iter->second;
   }
   return NULL; // don't throw exception (accessor)
}

inline Activity::Ptr ActivityManager::activityNew(const string &name) {
   Activity::Ptr activity = (*this)[name];
   if (activity) { throw NameInUseException(__FILE__, __LINE__, "name in use"); }
   
   activity = Activity::ActivityNew(this);
   (*this)[name] = activity;
  
   ActivityThread::Ptr thread = ActivityThread::ActivityThreadNew(activity);
   threads_.pushBack(thread);
   
   // ActivityReactor::Ptr reactor = ActivityReactor::ActivityReactorNew(activity, this);
   // activity_reactors_.elementIs(name, reactor);
   return activity;
}

inline void ActivityManager::activityDel(const string& name) {
   this->elementDel(name);
}


} /* end of namespace thread */
} /* end of namespoce Simone */
