#pragma once

#include <vector>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "../ptr.h"
#include "../pqueue.h"
#include "../map.h"

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
      void onNextTime() {}
      void onStatus() {}
   private:
      ActivityReactor(Activity::Ptr _notifier, ActivityManager *_owner) : 
                      Activity::Notifiee(_notifier), owner_(_owner){}
      ActivityManager *owner_;
   };
   
   // attributes =====================================================================
   Activity::Ptr activity(const string &name) const;
   
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
   
   // instantiating attributes -------------------------------------------------------
   Activity::Ptr activityNew(const string &name);
   void          activityDel(const string &name);
   // collection attributes ----------------------------------------------------------
   void pushActivity(Activity::Ptr);
private:
   ActivityManager() : time_delta_(0,0,0) {}
   
   PriorityQueue<Activity::Ptr,lt_Activity> activity_queue_;
                                                   // in addition to inherited Map
   // data members ===================================================================
   TimeDelta time_delta_;
   Map<string,ActivityReactor::Ptr> activity_reactors_;
   
   mutable boost::condition_variable_any time_delta_changed_;
   mutable boost::recursive_timed_mutex mutex_;
};

inline Activity::Ptr ActivityManager::activity(const string &name) const {
   const_iterator iter = this->element(name);
   if (iter != this->end()) { return iter->second; }
   return NULL; // don't throw exception (accessor)
}

// inline void ActivityManager::currentTimeIs(const Time& _time) {
//    // <nd name="if (_time > simulation_time_):">
//    //     <nd>fast forward to _time, executing activities with time less
//    //         than _time</nd>
//    // </nd>
//    // <nd name="else if (_time < simulation_time_)">
//    //     <nd name="if (activity currently executing)">
//    //         <nd name="interrupt & revert"></nd>
//    //     </nd>
//    // </nd>
//    
//    /* reference implementation:
//    while (!scheduledActivities_.empty()) {
//        
//        //figure out the next activity to run
//        Activity::Ptr nextToRun = scheduledActivities_.top();
// 
//        //if the next time is greater than the specified time, break
//        //the loop
//        if (nextToRun->nextTime() > t) {
//       break;
//        }
//        
//        //calculate amount of time to sleep
//        Time diff = Time(nextToRun->nextTime().value() - now_.value());
//        
//        //sleep 100ms (100,000 microseconds) for every unit of time
//        usleep(( ((int)diff.value()) * 100000));
//        
//        now_ = nextToRun->nextTime();
// 
//        //print out size of queue
//        cout << "size of queue: " << queue_->size() << endl;
// 
//        //run the minimum time activity and remove it from the queue
//        scheduledActivities_.pop();
// 
//        nextToRun->statusIs(Activity::executing);
//        nextToRun->statusIs(Activity::free);
// 
//    }
// 
//    //syncrhonize the time: */
//    current_time_ = _time;
// }

inline Activity::Ptr ActivityManager::activityNew(const string &name) {
   
   /* TODO:
      <nd name="assign thread to activity">
          <nd>sleep thread until exec time</nd>
      </nd>
    */
   Activity::Ptr activity = (*this)[name];
   if (activity) {
      cerr << "Error @ " << __FILE__ << ":" << __LINE__;
      cerr << " ~ activity already exists" << "\n";
      return NULL;
   }
   activity = Activity::ActivityNew(name, this);
   (*this)[name] = activity;
   
   ActivityReactor::Ptr reactor = ActivityReactor::ActivityReactorNew(activity, this);
   activity_reactors_.elementIs(name, reactor);
   
   return activity;
}

inline void ActivityManager::activityDel(const string& name) {
   Activity::Ptr activity = (*this)[name];
   // if (activity && activity->status() == Activity::kRunning) {
   //    activity->statusIs(Activity::kDeleted);
   // }
   /* TODO:
      <nd>deallocate assigned thread</nd>
      <nd>delete activity from pqueue</nd> */
   this->elementDel(name);
   activity_reactors_.elementDel(name);
}

inline void ActivityManager::pushActivity(Activity::Ptr _a) {
   activity_queue_.pushFront(_a);
}

} /* end of namespoce Chron */
