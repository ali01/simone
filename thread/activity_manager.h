#pragma once

#include <vector>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>

#include "../ptr.h"
#include "../deque.h"
#include "../map.h"

#include "activity_thread.h"
#include "activity.h"

using std::vector;

namespace Simone {

class ActivityManager : public Map<string,Activity::Ptr> {
   friend class Activity;
   typedef boost::recursive_timed_mutex::scoped_lock timed_lock;
public:
   typedef Simone::Ptr<const ActivityManager> PtrConst;
   typedef Simone::Ptr<ActivityManager> Ptr;

   static Ptr ActivityManagerNew() { return new ActivityManager(); }
   
   class ActivityReactor : public Activity::Notifiee {
   public:
      typedef Simone::Ptr<const ActivityReactor> PtrConst;
      typedef Simone::Ptr<ActivityReactor> Ptr;
      
      static Ptr ActivityReactorNew(Activity::Ptr _notifier, ActivityManager *_owner)
                                    { return new ActivityReactor(_notifier, _owner); }
      // member functions ------------------------------------------------------------
      const string& name() const { return notifier()->name(); }
      // overridden notifications ----------------------------------------------------
      void onStatus() {}
   private:
      ActivityReactor(Activity::Ptr _notifier, ActivityManager *_owner) : 
                      Activity::Notifiee(_notifier), owner_(_owner){}
      ActivityManager *owner_;
   };
   
   // attributes =====================================================================
   Time currentTime() const {
      return Time(Clock::kMicrosecUniversal) + time_delta_;
   }
   
   void timeDeltaInc(const TimeDelta& _delta) {
      {
         timed_lock lk(mutex_);
         time_delta_ += _delta;
      }
      time_delta_changed_.notify_all();
   }
   
   Activity::Ptr activity(const string &name) const;
   // instantiating attributes -------------------------------------------------------
   Activity::Ptr activityNew(const string &name);
   void          activityDel(const string &name);
private:
   ActivityManager() : time_delta_(0,0,0) {}
   // data members ===================================================================
   TimeDelta time_delta_;
   Deque<ActivityThread::Ptr> threads_;
   Map<string,ActivityReactor::Ptr> activity_reactors_;
   
   mutable boost::condition_variable_any time_delta_changed_;
   mutable boost::recursive_timed_mutex mutex_;
};

inline Activity::Ptr ActivityManager::activity(const string &name) const {
   const_iterator iter = this->element(name);
   if (iter != this->end()) { return iter->second; }
   return NULL; // don't throw exception (accessor)
}

inline Activity::Ptr ActivityManager::activityNew(const string &name) {
   Activity::Ptr activity = (*this)[name];
   if (activity) { throw NameInUseException(name); }
   
   activity = Activity::ActivityNew(name, this);
   (*this)[name] = activity;
   
   ActivityThread::Ptr thread = ActivityThread::ActivityThreadNew(activity);
   threads_.pushBack(thread);
   
   ActivityReactor::Ptr reactor = ActivityReactor::ActivityReactorNew(activity, this);
   activity_reactors_.elementIs(name, reactor);
   
   return activity;
}

inline void ActivityManager::activityDel(const string& name) {
   this->elementDel(name);
}

} /* end of namespoce Chron */
