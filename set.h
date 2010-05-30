/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once

#include <set>
#include <functional>
using std::set;
using std::less;
using std::allocator;

#include "utility.h"
#include "ptr_interface.h"

namespace Simone {

template <typename T,
          typename Compare=less<T>,
          typename Allocator=allocator<T> >
class Set :
  public Simone::PtrInterface<Set<T,Compare,Allocator> >, boost::noncopyable
{
public:
  typedef Simone::Ptr<const Set<T,Compare,Allocator> > PtrConst;
  typedef Simone::Ptr<Set<T,Compare,Allocator> > Ptr;

  typedef typename set<T,Compare,Allocator>::iterator iterator;
  typedef typename set<T,Compare,Allocator>::reverse_iterator reverse_iterator;
  typedef typename set<T,Compare,Allocator>::const_iterator const_iterator;
  typedef typename set<T,Compare,Allocator>::const_reverse_iterator
                                             const_reverse_iterator;

  static Ptr SetNew() { return new Set(); }
  Set() {}

  virtual ~Set() {}

  iterator begin() {
    return set_.begin();
  }

  iterator end() {
    return set_.end();
  }

  reverse_iterator rbegin() {
    return set_.rbegin();
  }

  reverse_iterator rend() {
    return set_.rend();
  }

  const_iterator begin() const {
    return set_.begin();
  }

  const_iterator end() const {
    return set_.end();
  }

  const_reverse_iterator rbegin() const {
    return set_.rbegin();
  }

  const_reverse_iterator rend() const {
    return set_.rend();
  }

  size_t size() const {
    return set_.size();
  }

  bool empty() const {
    return set_.empty();
  }

  iterator element(const T& _v) {
    return set_.find(_v);
  }

  const_iterator element(const T& _v) const {
    return set_.find(_v);
  }

  size_t count(const T& _v) const {
    return set_.count(_v);
  }

  void clear() {
    set_.clear();
  }

  void elementIs(const T& _v) {
    set_.insert(_v);
  }

  void elementDel(const T& _v) {
    set_.erase(_v);
  }

private:
  /* data members */
  set<T,Compare,Allocator> set_;
};

} /* end of namespace Simone */
