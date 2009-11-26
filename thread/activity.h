#pragma once

#include <string>
#include <functional>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "../time.h"
#include "../ptr.h"
#include "../pqueue.h"
#include "../exception.h"
#include "../utility.h"
#include "../numeric.h"
#include "../notifiee.h"
#include "../ordinal.h"
#include "../utility.h"

#include "this_thread.h"

using std::string;
using std::binary_function;

namespace Simone {
/*     forward declaration     */ class ActivityManager;
/*     forward declaration     */ class ActivityThread;
class Activity : public PtrInterface<Activity>, private boost::noncopyable {
   typedef boost::recursive_mutex::scoped_lock lock;
   typedef boost::recursive_timed_mutex::scoped_lock timed_lock;
   friend class ActivityThread;
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const Activity> PtrConst;
   typedef Simone::Ptr<Activity> Ptr;
   
   // TODO: consider kWaiting necessary?
   struct Config {
      enum ThreadStatus { kReady, kRunning };
      enum SchedulingMode { kDefault, kAutomatic };
      enum ExecutionMode { kNonBlocking, kBlocking };
   };
   struct Status { enum ThreadStatus { kReady, kStopping, kWaiting, kRunning }; };
   
   // factory constructor ============================================================
   static Ptr ActivityNew(const string &_name, const ActivityManager *const _m)
      { return new Activity(_name, _m); }
   
   class Notifiee : public BaseNotifiee<Activity> {
      friend class Activity;
   public:
      typedef Simone::Ptr<const Notifiee> PtrConst;
      typedef Simone::Ptr<Notifiee> Ptr;
      
      const Time& nextTime() const { return next_time_; }
      
      Activity::Config::SchedulingMode schedulingMode() const {
         return scheduling_mode_;
      }
      
      void schedulingModeIs(Activity::Config::SchedulingMode _m) {
         scheduling_mode_ = _m;
      }
      // notifications ---------------------------------------------------------------
      virtual void onStatus() {}
   protected:
      Notifiee(Activity::Ptr _a, Config::ExecutionMode _e=Config::kNonBlocking) :
                                                                      activity_(_a) {
         BaseNotifiee<Activity>::notifierIs(_a);
         processExecutionMode(_e);
      }
      
      Notifiee(Activity::Ptr _a, Time _next_time,
               Config::ExecutionMode _e=Config::kNonBlocking):
                                       next_time_(_next_time),
                                       activity_(_a),
                                       scheduling_mode_(Activity::Config::kDefault) {
         BaseNotifiee<Activity>::notifierIs(_a);
         processExecutionMode(_e);
      }
      Time next_time_;
      Activity::Ptr activity_;
      Activity::Config::SchedulingMode scheduling_mode_;
   private:
      void processExecutionMode(Config::ExecutionMode _e);
   };
   
   struct lt_NotifieePtr : public binary_function<Notifiee*, Notifiee*, bool> {
      bool operator()(Notifiee *a, Notifiee *b) const {
         return (a->nextTime() > b->nextTime());
      }
   };
   
   const string& name() const {
      lock lk(mutex_);
      return name_;
   }
   
   Status::ThreadStatus status() const {
      lock lk(mutex_);
      return status_;
   }
   
   void statusIs(Config::ThreadStatus _s) {
      lock lk(mutex_);
      switch (_s) {
         case Config::kReady:
            if (status_ != Status::kReady) {
               if (status_ == Status::kRunning) {
                  status_ = Status::kStopping;
                  fireOnStatus();
               }
            }
            break;
         case Config::kRunning:
            if (status_ != Status::kRunning) {
               runActivity();
            }
            break;
         default: throw AttributeNotSupportedException("unknown attribute");
      }
   }
   
   const TimeDelta& autoTimeSpacing() const { return auto_time_spacing_; }
   void autoTimeSpacingIs(const TimeDelta& _td) { auto_time_spacing_ = _td; }
   
   // notification support -----------------------------------------------------------
   void notifieeIs(Notifiee *_n) {
      if (_n->schedulingMode() == Config::kAutomatic) {
         _n->next_time_ = last_scheduled_time_ + autoTimeSpacing();
      } else if (_n->nextTime() == Time(Time::kNull)) {
         _n->next_time_ = Time(Clock::kMicrosecUniversal);
      }
      if (_n->nextTime() > last_scheduled_time_) {
         last_scheduled_time_ = _n->nextTime();
      }
      notifiees_.pushFront(_n);
   }
   
   void notifieeDel(Notifiee *_n) const {
      Activity *me = const_cast<Activity *>(this);
      PriorityQueue<Notifiee*>::iterator it = me->notifiees_.begin();
      for(; it != notifiees_.end(); ++it) { if (_n == *it) break; }
      if (it != notifiees_.end()) {
         me->notifiees_.elementDel(it);
      }
   }
   
protected:
   Activity(const string &_name, const ActivityManager *const _m) :
                                   name_(_name),
                                   status_(Status::kReady),
                                   last_scheduled_time_(Clock::kMicrosecUniversal),
                                   auto_time_spacing_(0,0,0),
                                   manager_(_m) {}
private:
   // member functions ===============================================================
   
   void runActivity();
   
   void fireOnStatus() {
      if (status() != Status::kRunning) {
         foreach(Notifiee *_n, notifiees_) { _n->onStatus(); }
      } else { ABORT(); }
   }
   
   void statusIs(Status::ThreadStatus _s) {
      lock lk(mutex_);
      status_ = _s;
   }
   
   TimeDelta timeDelta(const Time& _time) const;
   
   Time absoluteTime(const Time& _time) const;
   
   void sleepUntil(const Time& _time);
   // data members ===================================================================
   friend
   void Notifiee::processExecutionMode(Config::ExecutionMode _e);
   
   string name_;
   Status::ThreadStatus status_;
   Time last_scheduled_time_;
   TimeDelta auto_time_spacing_;
   const ActivityManager *const manager_;
   ActivityThread *activity_thread_;
   PriorityQueue<Notifiee*,lt_NotifieePtr> notifiees_;
   
   mutable boost::recursive_mutex mutex_; // coarse granularity sync mutex
};

} /* end of namespace Simone */
