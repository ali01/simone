/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once

#include <deque>
using std::deque;

#include "../ptr_interface.h"
#include "concurrent_collection.h"

namespace Simone {

template <typename T>
class ConcurrentDeque : public ConcurrentCollection {
protected:
   deque<T> deque_;
public:
   // type declarations ========================================================
   typedef Simone::Ptr<const ConcurrentDeque<T> > PtrConst;
   typedef Simone::Ptr<ConcurrentDeque<T> > Ptr;

   typedef typename deque<T>::iterator iterator;
   typedef typename deque<T>::reverse_iterator reverse_iterator;
   typedef typename deque<T>::const_iterator const_iterator;
   typedef typename deque<T>::const_reverse_iterator const_reverse_iterator;

   // factory constructor ======================================================
   static Ptr ConcurrentDequeNew() { return new ConcurrentDeque(); }
   ConcurrentDeque() {}
   virtual ~ConcurrentDeque() {
      ScopedLock lk(this->mutex());
   }

   // iterators ================================================================
   iterator begin()  {
      ScopedLock lk(this->mutex());
      return deque_.begin();
   }

   iterator end()    {
      ScopedLock lk(this->mutex());
      return deque_.end();
   }

   reverse_iterator rbegin() {
      ScopedLock lk(this->mutex());
      return deque_.rbegin();
   }

   reverse_iterator rend()   {
      ScopedLock lk(this->mutex());
      return deque_.rend();
   }


   const_iterator begin()  const {
      ScopedLock lk(this->mutex());
      return deque_.begin();
   }

   const_iterator end()    const {
      ScopedLock lk(this->mutex());
      return deque_.end();
   }

   const_reverse_iterator rbegin() const {
      ScopedLock lk(this->mutex());
      return deque_.rbegin();
   }

   const_reverse_iterator rend()   const {
      ScopedLock lk(this->mutex());
      return deque_.rend();
   }


   // accessors  ===============================================================
   size_t size() const {
      ScopedLock lk(this->mutex());
      return deque_.size();
   }

   bool empty() const {
      ScopedLock lk(this->mutex());
      return deque_.empty();
   }

   T& element(uint32_t _i) {
      ScopedLock lk(this->mutex());
      return deque_[_i];
   }

   T& front() {
      ScopedLock lk(this->mutex());
      return deque_.front();
   }

   T& back() {
      ScopedLock lk(this->mutex());
      return deque_.back();
   }


   const T& element(uint32_t _i) const {
      ScopedLock lk(this->mutex());
      return deque_[_i];
   }

   const T& front() const {
      ScopedLock lk(this->mutex());
      return deque_.front();
   }

   const T& back() const {
      ScopedLock lk(this->mutex());
      return deque_.back();
   }


   iterator elementDel(iterator it) {
      ScopedLock lk(this->mutex());
      return deque_.erase(it);
   }

   void elementIs(uint32_t _i, const T& _e) {
      ScopedLock lk(this->mutex());
      _e.collectionIs(this);
      deque_[_i] = _e;
   }


   void pushFront(const T& _e) {
      ScopedLock lk(this->mutex());
      deque_.push_front(_e);
   }

   void pushBack(const T& _e) {
      ScopedLock lk(this->mutex());
      deque_.push_back(_e);
   }

   void popFront() {
      ScopedLock lk(this->mutex());
      deque_.pop_front();
   }

   void popBack() {
      ScopedLock lk(this->mutex());
      deque_.pop_back();
   }

   void clear() {
      ScopedLock lk(this->mutex());
      deque_.clear();
   }


   T& operator[](const uint32_t& _i) {
      ScopedLock lk(this->mutex());
      return deque_[_i];
   }

   const T& operator[](const uint32_t& _i) const {
      ScopedLock lk(this->mutex());
      return deque_[_i];
   }
};


} /* end of namespace Simone */
