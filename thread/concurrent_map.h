/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once
#include <map>
#include <functional>
using std::map;
using std::less;

#include "scoped_lock.h"

#include "../ptr_interface.h"
#include "concurrent_collection.h"

namespace Simone {
namespace thread {

template <typename KeyType,
       typename ValueType,
       typename Cmp=less<KeyType> >
class ConcurrentMap : public ConcurrentCollection {
public:
  typedef Simone::Ptr<const ConcurrentMap<KeyType,ValueType,Cmp> > PtrConst;
  typedef Simone::Ptr<ConcurrentMap<KeyType,ValueType,Cmp> > Ptr;
  
  typedef typename map<KeyType,ValueType,Cmp>::iterator iterator;

  typedef typename map<KeyType,ValueType,Cmp>::reverse_iterator
                                                reverse_iterator;

  typedef typename map<KeyType,ValueType,Cmp>::const_iterator const_iterator;

  typedef typename map<KeyType,ValueType,Cmp>::const_reverse_iterator
                                                 const_reverse_iterator;
  
  static Ptr ConcurrentMapNew() { return new ConcurrentMap(); }
  ConcurrentMap() {}
  
  virtual ~ConcurrentMap() {
    ScopedLock lk(this->mutex());
  }
  
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
  
  size_t size() const {
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
  
  void elementIs(const KeyType& _key, const ValueType& _v) {
    ScopedLock lk(this->mutex());
    _v.collectionIs(this);
    map_[_key] = _v;
  }
  
  void elementDel(iterator _it) {
    ScopedLock lk(this->mutex());
    map_.erase(_it);
  }
  void elementDel(const KeyType& _key) {
    ScopedLock lk(this->mutex());
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
  /* data members */
  map<KeyType,ValueType,Cmp> map_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
