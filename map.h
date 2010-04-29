/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once
#include <map>
#include <functional>
using std::map;
using std::less;

#include "ptr_interface.h"
#include "utility.h"

namespace Simone {

template<typename KeyT,typename ValueT,typename Cmp=less<KeyT> >
class Map : public PtrInterface<Map<KeyT,ValueT,Cmp> >, boost::noncopyable {
public:
  typedef Simone::Ptr<const Map<KeyT,ValueT,Cmp> > PtrConst;
  typedef Simone::Ptr<Map<KeyT,ValueT,Cmp> > Ptr;
  
  typedef typename map<KeyT,ValueT,Cmp>::iterator iterator;

  typedef typename map<KeyT,ValueT,Cmp>::reverse_iterator reverse_iterator;

  typedef typename map<KeyT,ValueT,Cmp>::const_iterator const_iterator;

  typedef typename map<KeyT,ValueT,Cmp>::const_reverse_iterator
                                           const_reverse_iterator;
  
  static Ptr MapNew() { return new Map(); }
  
  Map() {}
  
  virtual ~Map() {}
  
  iterator begin() {
    return map_.begin();
  }
  
  iterator end() {
    return map_.end();
  }
  
  reverse_iterator rbegin() {
    return map_.rbegin();
  }
  
  reverse_iterator rend() {
    return map_.rend();
  }
  
  const_iterator begin() const {
    return map_.begin();
  }
  
  const_iterator end() const {
    return map_.end();
  }
  
  const_reverse_iterator rbegin() const {
    return map_.rbegin();
  }
  
  const_reverse_iterator rend()   const {
    return map_.rend();
  }
  
  size_t size() const {
    return map_.size();
  }
  
  bool empty() const {
    return map_.empty();
  }
  
  iterator element(const KeyT& _key) {
    return map_.find(_key);
  }
  const_iterator element(const KeyT& _key) const {
    return map_.find(_key);
  }
  
  void elementIs(const KeyT& _key, const ValueT& _v) {
    map_[_key] = _v;
  }
  
  void elementDel(iterator _it) {
    map_.erase(_it);
  }
  void elementDel(const KeyT& _key) {
    map_.erase(_key);
  }
  
  ValueT& operator[](const KeyT& key) {
    return map_[key];
  }
  
  const ValueT& operator[](const KeyT& key) const {
    return map_[key];
  }
private:
  /* data members */
  map<KeyT,ValueT,Cmp> map_;
};

} /* end of namespace Simone */
