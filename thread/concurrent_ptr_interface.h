/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a 
   Modeling & Simulation's Perspective ~ Chapter 5: Memory management with smart
   pointers. */

#pragma once

#include "recursive_mutex.h"
#include "scoped_lock.h"

#include "../ptr.h"

namespace Simone {
namespace thread {

template <typename T>
class ConcurrentPtrInterface : public Simone::PtrInterface<T> {
   friend class Ptr<T>;
public:
   const ConcurrentPtrInterface *newRef() const {
      ScopedLock lk(mutex_);
      return static_cast<const ConcurrentPtrInterface*>(PtrInterface<T>::newRef());
   }
   
   const ConcurrentPtrInterface *deleteRef() const {
      mutex_.lock();
      if (ref_ == 0) {
         mutex_.unlock();
         throw MemoryException(__FILE__, __LINE__, "attempt to delete an object with zero references");
      } else if( --ref_ == 0 ) {
         mutex_.unlock();
         this->onZeroReferences();
         return 0;
      } else {
         mutex_.unlock();
      }
      return this;
   }
   
   virtual RecursiveMutex& mutex() const {
      return mutex_;
   }
protected:
   ConcurrentPtrInterface() {}
   virtual ~ConcurrentPtrInterface() { ScopedLock lk(this->mutex()); }
   using PtrInterface<T>::ref_;
private:
   mutable RecursiveMutex mutex_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
