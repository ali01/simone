/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#include "scoped_lock.h"
#include "recursive_mutex.h"

namespace Simone {

ScopedLock::ScopedLock(RecursiveMutex& _l) : mutex_(&_l), owns_lock_(true) {
   if (mutex_) {
      mutex_->lock();
   } else {
      string msg = "null pointer passed to ScopedLock's constructor";
      throw NullPointerException(__FILE__, __LINE__, msg);
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
      throw Simone::InvalidOperationException(__FILE__, __LINE__, ss.str());
   }
   mutex_->lock();
   owns_lock_ = true;
}

void ScopedLock::unlock() {
   if ( ! owns_lock_) {
      stringstream ss;
      cerr << __FILE__ << ":" << __LINE__ << endl;
      ss << "attempt to unlock an already unlocked mutex through ScopedLock";
      throw Simone::InvalidOperationException(__FILE__, __LINE__, ss.str());
   }
   mutex_->unlock();
   owns_lock_ = false;
}

} /* end of namespace Simone */
