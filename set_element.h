#pragma once
#include "ptr.h"

namespace Simone {

template <typename Collection>
class ThreadSafeCollectionElement : public Simone::Ptr<ThreadSafeCollectionElement> {
public:
   void addToCollection(Collection *_c) {
      
   }
protected:
   ConcurrentSetElement() {}
   virtual ~ConcurrentSetElement() {}
   
   // Collection
};

} /* end of namespace Simone */

