/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once

#include "../utility.h"

#include "concurrent_collection_element.h"
#include "concurrent_ptr_interface.h"
#include "recursive_mutex.h"
#include "scoped_lock.h"

namespace Simone {
namespace thread {

class ConcurrentCollection : public ConcurrentPtrInterface<ConcurrentCollection>,
                             private boost::noncopyable {
public:
   RecursiveMutex& mutex() const {
      return mutex_;
   }
protected:
   ConcurrentCollection() {}
   virtual ~ConcurrentCollection() {
      ScopedLock lk(mutex_);
   }
   
   mutable RecursiveMutex mutex_;
};

} /* end of namespace thread */
} /* end of namespace Simone */

// concurrency ====================================================================
// TODO: define iterators that lock the set upon creation and unlock the set when
// deleted
