#pragma once

#include <boost/thread/recursive_mutex.hpp>
#include "mutex.h"

#include "../exception.h"

namespace Simone {
namespace thread {

class RecursiveMutex : public Mutex<boost::recursive_mutex> {
public:
   RecursiveMutex() {}
   virtual ~RecursiveMutex() {}
   
   struct config {
      enum lock { kLocked, kUnlocked };
   };
   
   struct status {
      enum lock { kLocked, kUnlocked };
   };
   
   // virtual void lockStatusIs(config::lock _s) {
   //    lockStatusIs((int)_s);
   // }
protected:
   
private:
   // void lockStatusIs(int _s) {
   //    switch (_s) {
   //       case config::kLocked:
   //          mutex_.lock();
   //          break;
   //       case config::kUnlocked:
   //          mutex_.unlock();
   //          break;
   //       default:
   //          throw UnknownAttrException();
   //    }
   // }
};

} /* end of namespace thread */
} /* end of namespace Simone */
