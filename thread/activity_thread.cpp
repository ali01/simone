#include "activity_thread.h"
#include "../utility.h"

#include "../ptr.h"
#include "../exception.h"

#include "activity.h"

#include <boost/thread/thread.hpp>

namespace Simone {

void ActivityThread::statusIs(Config::Attr _status) {
   switch (_status) {
      case Config::kFree:
         runnable_.statusIs(RunnableActivity::Config::kFree);
         if (thread_) {
            delete thread_;
            thread_ = NULL;
         }
         break;
      case Config::kRunning:
         switch (runnable_.status()) {
            case RunnableActivity::Status::kFree:
               {
                  string msg ="incomplete initialization of ActivityThread";
                  throw IncompleteInitializationException(msg);
               }
            case RunnableActivity::Status::kReady:
               assert(thread_ == NULL);
               thread_ = new boost::thread(runnable_);
               break;
            case RunnableActivity::Status::kRunning:
               assert(thread_);
               break;
            default: throw AttributeNotSupportedException("attribute unknown");
         }
         break;
      default: throw AttributeNotSupportedException("attribute unknown");
   }
}

void ActivityThread::RunnableActivity::statusIs(Config::Attr _s) {
   if (_s == Config::kFree || _s == Config::kReady) {
      activity_->statusIs(Activity::Config::kReady);
      if (_s == Config::kFree) { activity_ = NULL; }
      return;
   }
   throw AttributeNotSupportedException("attribute unknown");
}

} /* end of namespace Simone */
