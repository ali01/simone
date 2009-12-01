#pragma once
#include <map>
using std::map;

#include "scoped_lock.h"

#include "../ptr_interface.h"
#include "concurrent_collection.h"

namespace Simone {
namespace thread {

template <typename KeyType, typename ValueType>
class ConcurrentMap : public ConcurrentCollection {
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const ConcurrentMap<KeyType,ValueType> > PtrConst;
   typedef Simone::Ptr<ConcurrentMap<KeyType,ValueType> > Ptr;
   
   typedef typename map<KeyType,ValueType>::iterator iterator;
   typedef typename map<KeyType,ValueType>::reverse_iterator 
                                                          reverse_iterator;
   typedef typename map<KeyType,ValueType>::const_iterator
                                                          const_iterator;
   typedef typename map<KeyType,ValueType>::const_reverse_iterator
                                                          const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr ConcurrentMapNew() { return new ConcurrentMap(); }
   ConcurrentMap() {}
   virtual ~ConcurrentMap() { ScopedLock lk(this->mutex()); }
   
   // iterators ======================================================================
   iterator begin() {
      ScopedLock lk(this->mutex());
      return map_.begin();
   }
   
   iterator end() {
      ScopedLock lk(this->mutex());
      return map_.end();
   }
   
   reverse_iterator rbegin() {
      ScopedLock lk(this->mutex());
      return map_.rbegin();
   }
   
   reverse_iterator rend() {
      ScopedLock lk(this->mutex());
      return map_.rend();
   }
   
   const_iterator begin() const {
      ScopedLock lk(this->mutex());
      return map_.begin();
   }
   
   const_iterator end() const {
      ScopedLock lk(this->mutex());
      return map_.end();
   }
   
   const_reverse_iterator rbegin() const {
      ScopedLock lk(this->mutex());
      return map_.rbegin();
   }
   
   const_reverse_iterator rend()   const {
      ScopedLock lk(this->mutex());
      return map_.rend();
   }
   
   // accessors  =====================================================================
   size_t size()  const {
      ScopedLock lk(this->mutex());
      return map_.size();
   }
   
   bool empty() const {
      ScopedLock lk(this->mutex());
      return map_.empty();
   }
   
   iterator element(const KeyType& _key) {
      ScopedLock lk(this->mutex());
      return map_.find(_key);
   }
   const_iterator element(const KeyType& _key) const {
      ScopedLock lk(this->mutex());
      return map_.find(_key);
   }
   
   // mutators =======================================================================
   void elementIs(const KeyType& _key, const ValueType& _v) {
      _v->collectionIs(this);
      map_[_key] = _v;
   }
   
   void elementDel(iterator _it) {
      ScopedLock lk(this->mutex());
      map_.erase(_it);
   }
   void elementDel(const KeyType& _key) {
      map_.erase(_key);
   }
   
   ValueType& operator[](const KeyType& key) {
      ScopedLock lk(this->mutex());
      return map_[key];
   }
   
   const ValueType& operator[](const KeyType& key) const {
      ScopedLock lk(this->mutex());
      return map_[key];
   }
private:
   // member functions ===============================================================
   // data members ===================================================================
   map<KeyType,ValueType> map_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
