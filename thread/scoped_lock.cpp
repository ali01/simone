#include "scoped_lock.h"
#include "recursive_mutex.h"

namespace Simone {
namespace thread {

ScopedLock::ScopedLock(RecursiveMutex& _l) : mutex_(&_l), owns_lock_(true) {
   if (mutex_) {
      mutex_->lock();
   } else {
      stringstream ss;
      cerr << __FILE__ << ":" << __LINE__ << endl;
      ss << "null pointer passed to ScopedLock's constructor";
      throw NullPointerException(ss.str());
   }
}

ScopedLock::~ScopedLock() {
   assert(mutex_);
   assert(owns_lock_);
   mutex_->unlock();
}

void ScopedLock::lock() {
   if (owns_lock_) {
      stringstream ss;
      cerr << __FILE__ << ":" << __LINE__ << endl;
      ss << "attempt to recursively lock a mutex through ScopedLock";
      throw Simone::InvalidOperationException(ss.str());
   }
   mutex_->lock();
   owns_lock_ = true;
}

void ScopedLock::unlock() {
   if ( ! owns_lock_) {
      stringstream ss;
      cerr << __FILE__ << ":" << __LINE__ << endl;
      ss << "attempt to unlock an already unlocked mutex through ScopedLock";
      throw Simone::InvalidOperationException(ss.str());
   }
   mutex_->unlock();
   owns_lock_ = false;
}

} /* end of namespace thread */
} /* end of namespace Simone */
