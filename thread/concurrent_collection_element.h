/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once

#include "concurrent_ptr_interface.h"
#include "recursive_mutex.h"
#include "scoped_lock.h"

namespace Simone {

class ConcurrentCollectionElement :
  public ConcurrentPtrInterface<ConcurrentCollectionElement>
{
public:
   template <typename Collection>
   void collectionIs(Collection *_c) const {
      this->mutex().parentMutexIs(_c->mutex());
   }

   virtual RecursiveMutex& mutex() const {
      return this->mutex();
   }
protected:
   ConcurrentCollectionElement() {}
   virtual ~ConcurrentCollectionElement() {
      ScopedLock lk(this->mutex());
   }
};

} /* end of namespace Simone */
