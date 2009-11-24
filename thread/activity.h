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
class Activity : public PtrInterface<Activity>, private boost::noncopyable {
   typedef boost::recursive_mutex::scoped_lock lock;
   typedef boost::recursive_timed_mutex::scoped_lock timed_lock;
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const Activity> PtrConst;
   typedef Simone::Ptr<Activity> Ptr;
   
   // TODO: consider kWaiting necessary?
   struct Config { enum Attr { kReady, kRunning }; };
   struct Status { enum Attr { kReady, kStopping, kWaiting, kRunning }; };
   // factory constructor ============================================================
   static Ptr ActivityNew(const string &_name, const ActivityManager *const _m)
      { return new Activity(_name, _m); }
   
   class Notifiee : public BaseNotifiee<Activity> {
      friend class Activity;
   public:
      typedef Simone::Ptr<const Notifiee> PtrConst;
      typedef Simone::Ptr<Notifiee> Ptr;
      
      const Time& nextTime() const { return next_time_; }
      // notifications ---------------------------------------------------------------
      virtual void onStatus() {}
      virtual void onNextTime() {}
   protected:
      Notifiee(Activity::Ptr _a) : BaseNotifiee<Activity>(_a) {}
      Notifiee(Activity::Ptr _a, Time _next_time): BaseNotifiee<Activity>(_a),
                                                   next_time_(_next_time) {}
      Time next_time_;
   };
   
   struct lt_NotifieePtr : public binary_function<Notifiee*, Notifiee*, bool> {
      bool operator()(Notifiee *a, Notifiee *b) const {
         return (a->nextTime() > b->nextTime());
      }
   };
   
   // TODO: consider how to support this
   // void transferRateIs(const ShipmentsPerDay& _s) { transfer_rate_ = _s; }
   
   const string& name() const {
      lock lk(mutex_);
      return name_;
   }
   
   Status::Attr status() const {
      lock lk(mutex_);
      return status_;
   }
   
   void statusIs(Config::Attr _s) {
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
   
   Time nextTime() const {
      lock lk(mutex_);
      return next_time_;
   }
   
   void nextTimeIs(const Time& _t) {
      lock lk(mutex_);
      next_time_ = _t;
      fireOnNextTime();
   }
   
   // notification support -----------------------------------------------------------
   void notifieeIs(Notifiee *_n) const {
      Activity *me = const_cast<Activity *>(this);
      if (_n->nextTime() == Time(Time::kNull))
         _n->next_time_ = Time(Clock::kMicrosecUniversal);
      me->notifiees_.pushFront(_n);
   }
   
   void notifieeDel(Notifiee *_n) const {
      Activity *me = const_cast<Activity *>(this);
      PriorityQueue<Notifiee*>::iterator it = me->notifiees_.begin();
      for(; it != notifiees_.end(); ++it) { if (_n == *it) break; }
      if (it != notifiees_.end()) {
         me->notifiees_.elementDel(it);
      }
   }
   // overloaded operators ===========================================================
protected:
   Activity(const string &_name, const ActivityManager *const _m) :
                                   name_(_name),
                                   status_(Status::kReady),
                                   next_time_(Clock::kMicrosecUniversal),
                                   manager_(_m) {}
private:
   // member functions ===============================================================
   
   void runActivity();
   
   void fireOnStatus() {
      if (status() != Status::kRunning) {
         foreach(Notifiee *_n, notifiees_) { _n->onStatus(); }
      } else { ABORT(); }
   }
   
   void fireOnNextTime() { foreach(Notifiee *_n, notifiees_) { _n->onNextTime(); }}
   
   void statusIs(Status::Attr _s) {
      lock lk(mutex_);
      status_ = _s;
   }
   
   TimeDelta timeDelta(const Time& _time) const;
   
   Time absoluteTime(const Time& _time) const;
   
   void sleepUntil(const Time& _time);
   // data members ===================================================================
   
   string name_;
   Status::Attr status_;
   Time next_time_;
   PriorityQueue<Notifiee*,lt_NotifieePtr> notifiees_;
   const ActivityManager *const manager_;
   
   mutable boost::recursive_mutex mutex_; // coarse granularity sync mutex
};

class lt_Activity : public binary_function<Activity::Ptr, Activity::Ptr, bool> {
public:
   lt_Activity() {}
   bool operator()(Activity::Ptr a, Activity::Ptr b) const {
      return (a->nextTime() > b->nextTime());
   }
};

} /* end of namespace Simone */
