/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once

#include "recursive_mutex.h"
#include "scoped_lock.h"

namespace Simone {

class ConcurrentCollectionElement  {
public:
   template <typename Collection>
   void collectionIs(Collection *_c) const {
      mutex().parentMutexIs(_c->mutex());
   }
   
   virtual RecursiveMutex& mutex() const {
      return mutex_;
   }
protected:
   ConcurrentCollectionElement() {}
   virtual ~ConcurrentCollectionElement() {
      ScopedLock lk(this->mutex());
   }
private:
   mutable RecursiveMutex mutex_;
};

} /* end of namespace Simone */
