#pragma once

#include "concurrent_collection_element.h"
#include "recursive_mutex.h"
#include "scoped_lock.h"

namespace Simone {
namespace thread {

class ConcurrentCollectionElement :
                          public ConcurrentPtrInterface<ConcurrentCollectionElement> {
public:
   template <typename Collection>
   void collectionIs(Collection *_c) const {
      mutex().parentMutexIs(_c->mutex());
   }
protected:
   ConcurrentCollectionElement() {}
   virtual ~ConcurrentCollectionElement() {
      ScopedLock lk(this->mutex());
   }
private:
};

} /* end of namespace thread */
} /* end of namespace Simone */
