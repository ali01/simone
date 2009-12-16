/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once
#include "../utility.h"

#include "../ptr_interface.h"
#include "../exception.h"

#include "activity.h"

#include <boost/thread/thread.hpp>

namespace Simone {
namespace thread {

class ActivityThread : public PtrInterface<ActivityThread>, boost::noncopyable {
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const ActivityThread> PtrConst;
   typedef Simone::Ptr<ActivityThread> Ptr;
   
   // factory constructor ============================================================
   static Ptr ActivityThreadNew(Activity::Ptr _a) { return new ActivityThread(_a); }
   
   // member functions ===============================================================
   Activity::Ptr activity() const {
      return runnable_.activity();
   }
   
   Activity::status::RunStatus runStatus() const {
      return activity()->runStatus();
   }
   
   void runStatusIs(Activity::config::RunStatus _s) {
      activity()->runStatusIs(_s);
   }
protected:
   ActivityThread(Activity::Ptr _a) : runnable_(_a), thread_(runnable_) {
      _a->threads_.add_thread(&thread_);
   }
private:
   class RunnableActivity {
   public:      
      RunnableActivity(Activity::Ptr _a) : activity_(_a) {
         if (activity_ == NULL) throw NullPointerException(__FILE__, __LINE__, "null activity");
      }
      
      Activity::Ptr activity() const { return activity_; }
      
      void operator()() {
         activity_->runStatusIs(Activity::config::kRunning);
      }
   private:
      Activity::Ptr activity_;
   };
   
   // data members ===================================================================
   RunnableActivity  runnable_;
   boost::thread     thread_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
