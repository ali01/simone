/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#ifndef DEQUE_H_1UNC3N06
#define DEQUE_H_1UNC3N06


#include <deque>
using std::deque;

#include "ptr_interface.h"
#include "utility.h"

namespace Simone {

template <typename T>
class Deque : public PtrInterface<Deque<T> >, boost::noncopyable {
protected:
   deque<T> deque_;
public:
   // type declarations ========================================================
   typedef Simone::Ptr<const Deque<T> > PtrConst;
   typedef Simone::Ptr<Deque<T> > Ptr;

   typedef typename deque<T>::iterator iterator;
   typedef typename deque<T>::reverse_iterator reverse_iterator;
   typedef typename deque<T>::const_iterator const_iterator;
   typedef typename deque<T>::const_reverse_iterator const_reverse_iterator;

   // factory constructor ======================================================
   static Ptr DequeNew() { return new Deque(); }
   Deque() {}
   virtual ~Deque() {}

   // iterators ================================================================
   iterator begin()  { return deque_.begin(); }

   iterator end()    { return deque_.end(); }

   reverse_iterator rbegin() { return deque_.rbegin(); }

   reverse_iterator rend()   { return deque_.rend(); }


   const_iterator begin()  const { return deque_.begin(); }

   const_iterator end()    const { return deque_.end(); }

   const_reverse_iterator rbegin() const { return deque_.rbegin(); }

   const_reverse_iterator rend()   const { return deque_.rend(); }


   // accessors  ===============================================================
   size_t size() const { return deque_.size(); }

   bool empty() const { return deque_.empty(); }

   T& element(uint32_t _i) { return deque_[_i]; }

   T& front() { return deque_.front(); }

   T& back() { return deque_.back(); }


   const T& element(uint32_t _i) const { return deque_[_i]; }

   const T& front() const { return deque_.front(); }

   const T& back() const { return deque_.back(); }


   iterator elementDel(iterator it) { return deque_.erase(it); }

   void elementIs(uint32_t _i, const T& _e) {
      _e.collectionIs(this);
      deque_[_i] = _e;
   }


   void pushFront(const T& _e) { deque_.push_front(_e); }

   void pushBack(const T& _e) { deque_.push_back(_e); }

   void popFront() { deque_.pop_front(); }

   void popBack() { deque_.pop_back(); }

   void clear() { deque_.clear(); }


   T& operator[](const uint32_t& _i) { return deque_[_i]; }

   const T& operator[](const uint32_t& _i) const { return deque_[_i]; }
};


} /* end of namespace thread */
} /* end of namespace Simone */


#endif
