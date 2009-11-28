#pragma once
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>

#include <boost/thread/recursive_mutex.hpp>

#include "../ptr.h"
#include "../deque.h"

using std::priority_queue;
using std::deque;
using std::less;

namespace Simone {
template <typename T, typename Compare=less<typename deque<T>::value_type> >
class ConcurrentPriorityQueue : private Deque<T> {
   typedef boost::recursive_mutex::scoped_lock scoped_lock_t;
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const ConcurrentPriorityQueue<T,Compare> > PtrConst;
   typedef Simone::Ptr<ConcurrentPriorityQueue<T,Compare> > Ptr;
   
   typedef typename deque<T>::iterator               iterator;
   typedef typename deque<T>::reverse_iterator       reverse_iterator;
   typedef typename deque<T>::const_iterator         const_iterator;
   typedef typename deque<T>::const_reverse_iterator const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr ConcurrentPriorityQueueNew() { return new ConcurrentPriorityQueue(); }
   ConcurrentPriorityQueue() : queue_(Compare(), Deque<T>::deque_) {}
   ~ConcurrentPriorityQueue() { scoped_lock_t lk(mutex_); }
   
   // iterators ======================================================================
   iterator begin() {
      scoped_lock_t lk(mutex_);
      return Deque<T>::deque_.begin();
   }
   
   iterator end() {
      scoped_lock_t lk(mutex_);
      return Deque<T>::deque_.end();
   }
   
   reverse_iterator rbegin() {
      scoped_lock_t lk(mutex_);
      return Deque<T>::deque_.rbegin();
   }
   
   reverse_iterator rend() {
      scoped_lock_t lk(mutex_);
      return Deque<T>::deque_.rend();
   }
   
   const_iterator begin() const {
      scoped_lock_t lk(mutex_);
      return Deque<T>::deque_.begin();
   }
   
   const_iterator end() const {
      scoped_lock_t lk(mutex_);
      return Deque<T>::deque_.end();
   }
   
   const_reverse_iterator rbegin() const {
      scoped_lock_t lk(mutex_);
      return Deque<T>::deque_.rbegin();
   }
   
   const_reverse_iterator rend() const {
      scoped_lock_t lk(mutex_);
      return Deque<T>::deque_.rend();
   }
   
   // accessors  =====================================================================
   size_t size() const {
      scoped_lock_t lk(mutex_);
      return queue_.size();
   }
   
   bool empty() const {
      scoped_lock_t lk(mutex_);
      return queue_.empty();
   }
   
   using Deque<T>::element;
   
   const T& front() {
      scoped_lock_t lk(mutex_);
      return queue_.top();
   }
   
   iterator elementDel(iterator it) {
      scoped_lock_t lk(mutex_);
      iterator ret = Deque<T>::elementDel(it);
      heapify();
      return ret;
   }
   
   void elementIs(uint32_t _i, const T& _e) {
      scoped_lock_t lk(mutex_);
      Deque<T>::elementIs(_i, _e);
      heapify();
   }
   
   void push(const T& _e) {
      scoped_lock_t lk(mutex_);
      queue_.push(_e);
   }
   
   void popFront() {
      scoped_lock_t lk(mutex_);
      queue_.pop();
   }
   
   void clear() {
      scoped_lock_t lk(mutex_);
      Deque<T>::clear();
      heapify();
   }
   
protected:
private:
   void heapify() {
      scoped_lock_t lk(mutex_);
      std::make_heap(Deque<T>::begin(), Deque<T>::end(), Compare());
   }
   // member functions ===============================================================
   // data members ===================================================================
   priority_queue<T,deque<T>,Compare> queue_;
   mutable boost::recursive_mutex mutex_;
};

} /* end of namespace Simone */