#pragma once

#include <set>
#include <functional>
using std::set;
using std::less;
using std::allocator;

#include "recursive_mutex.h"
#include "scoped_lock.h"

#include "../ptr_interface.h"
#include "concurrent_collection.h"

namespace Simone {
namespace thread {

template <typename T,
          typename Compare=less<T>,
          typename Allocator=allocator<T> >
class ConcurrentSet : public ConcurrentCollection {
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const ConcurrentSet<T,Compare,Allocator> > PtrConst;
   typedef Simone::Ptr<ConcurrentSet<T,Compare,Allocator> > Ptr;
   
   typedef typename set<T,Compare,Allocator>::iterator iterator;
   typedef typename set<T,Compare,Allocator>::reverse_iterator reverse_iterator;
   typedef typename set<T,Compare,Allocator>::const_iterator const_iterator;
   typedef typename set<T,Compare,Allocator>::const_reverse_iterator
                                                               const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr ConcurrentSetNew() { return new ConcurrentSet(); }
   ConcurrentSet() {} // allow static allocation
   virtual ~ConcurrentSet() {
      ScopedLock lk(this->mutex());
   }
   
   // iterators ======================================================================
   iterator begin() {
      ScopedLock lk(this->mutex());
      return set_.begin();  
   }
   
   iterator end() {
      ScopedLock lk(this->mutex());
      return set_.end();    
   }
   
   reverse_iterator rbegin() {
      ScopedLock lk(this->mutex());
      return set_.rbegin(); 
   }
   
   reverse_iterator rend() {
      ScopedLock lk(this->mutex());
      return set_.rend();   
   }
   
   
   const_iterator begin() const {
      ScopedLock lk(this->mutex());
      return set_.begin();  
   }

   const_iterator end() const {
      ScopedLock lk(this->mutex());
      return set_.end();    
   }

   const_reverse_iterator rbegin() const {
      ScopedLock lk(this->mutex());
      return set_.rbegin(); 
   }

   const_reverse_iterator rend() const {
      ScopedLock lk(this->mutex());
      return set_.rend();   
   }

   
   // accessors  =====================================================================
   size_t size() const {
      ScopedLock lk(this->mutex());
      return set_.size();  
   }

   bool empty() const {
      ScopedLock lk(this->mutex());
      return set_.empty(); 
   }

   
   iterator element(const T& _v) {
      ScopedLock lk(this->mutex());
      return set_.find(_v); 
   }

   const_iterator element(const T& _v) const {
      ScopedLock lk(this->mutex());
      return set_.find(_v); 
   }

   
   size_t count(const T& _v) const {
      ScopedLock lk(this->mutex());
      return set_.count(_v); 
   }

   
   // mutators =======================================================================
   void elementIs(const T& _v) {
      _v->collectionIs(this);
      ScopedLock lk(this->mutex());
      set_.insert(_v); 
   }

   void elementDel(const T& _v) {
      ScopedLock lk(this->mutex());
      set_.erase(_v);
   }
private:
   // member functions ===============================================================
   // data members ===================================================================
   set<T,Compare,Allocator> set_;
};



} /* end of namespace thread */
} /* end of namespace Simone */
