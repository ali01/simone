#pragma once

#include "concurrent_collection_element.h"
#include "recursive_mutex.h"
#include "scoped_lock.h"

namespace Simone {
namespace thread {

// TODO: dual recursive_mutex
class ConcurrentCollectionElement :
                          public ConcurrentPtrInterface<ConcurrentCollectionElement> {
public:
   template <typename Collection>
   void collectionIs(Collection *_c) const {
      mutex().parentMutexIs(_c->mutex());
      ScopedLock lk(this->mutex());
      collection_mutex_ = &_c->mutex();
   }
protected:
   ConcurrentCollectionElement() {}
   virtual ~ConcurrentCollectionElement() {
      ScopedLock lk(this->mutex());
   }
private:
   mutable RecursiveMutex *collection_mutex_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
