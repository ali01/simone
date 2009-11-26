#pragma once
#include "../utility.h"

#include "../ptr.h"
#include "../exception.h"

#include "activity.h"

#include <boost/thread/thread.hpp>

namespace Simone {

class ActivityThread : public PtrInterface<ActivityThread> {
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const ActivityThread> PtrConst;
   typedef Simone::Ptr<ActivityThread> Ptr;
   
   struct Config { enum ThreadStatus { kFree, kRunning }; };
   struct Status { enum ThreadStatus { kFree, kReady, kRunning }; };
   // factory constructor ============================================================
   static Ptr ActivityThreadNew()                 { return new ActivityThread();   }
   static Ptr ActivityThreadNew(Activity::Ptr _a) { return new ActivityThread(_a); }
   
   ~ActivityThread() { statusIs(Config::kFree); }
   // member functions ===============================================================
   Status::ThreadStatus status() const {
      if (thread_ == NULL) { return Status::kFree; }
      else {
         return thread_ ? Status::kRunning : Status::kReady;
      }
   }
   
   void statusIs(Config::ThreadStatus _status);
   void executionModeIs(Activity::Config::ExecutionMode _e) {
      switch (_e) {
         case Activity::Config::kBlocking:
            if (thread_) thread_->join();
            break;
         case Activity::Config::kNonBlocking:
            break;
         default: throw AttributeNotSupportedException("unknown execution mode");
      }
   }
protected:
   ActivityThread() : runnable_(NULL), thread_(NULL) {}
   ActivityThread(Activity::Ptr _a) : runnable_(_a) {
      thread_ = new boost::thread(runnable_);
      _a->activity_thread_ = this;
   }
private:
   class RunnableActivity {
   public:      
      RunnableActivity(Activity::Ptr _a) : activity_(_a) {}
      
      struct Config { enum ThreadStatus { kFree, kReady }; };
      struct Status { enum ThreadStatus { kFree, kReady, kRunning }; };
      
      Status::ThreadStatus status() const {
         if (activity_ == NULL) { return Status::kFree; }
         else {
            bool running = activity_->status() == Activity::Status::kRunning;
            return running ? Status::kRunning : Status::kReady;
         }
      }
      
      void statusIs(Config::ThreadStatus _s);
      
      Activity::Ptr activity() const { return activity_; }
      
      void activityIs(Activity::Ptr _a) {
         activity_->statusIs(Activity::Config::kReady);
         activity_ = _a;
      }
      
      void operator()() {
         activity_->statusIs(Activity::Config::kRunning);
      }
   private:
      Activity::Ptr activity_;
   };
   
   // member functions ===============================================================
   Activity::Ptr activity() const { return runnable_.activity(); }
   
   void activityIs(Activity::Ptr _a) { runnable_.activityIs(_a); }
   // data members ===================================================================
   RunnableActivity  runnable_;
   boost::thread    *thread_;
   // disallowed operations ==========================================================
   ActivityThread(const ActivityThread&);
   void operator=(const ActivityThread&);
};

} /* end of namespace Simone */
