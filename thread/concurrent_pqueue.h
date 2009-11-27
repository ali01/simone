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
   typedef boost::recursive_mutex::scoped_lock lock;
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
   ~ConcurrentPriorityQueue() { lock lk(mutex_); }
   
   // iterators ======================================================================
   iterator begin() {
      lock lk(mutex_);
      return Deque<T>::deque_.begin();
   }
   
   iterator end() {
      lock lk(mutex_);
      return Deque<T>::deque_.end();
   }
   
   reverse_iterator rbegin() {
      lock lk(mutex_);
      return Deque<T>::deque_.rbegin();
   }
   
   reverse_iterator rend() {
      lock lk(mutex_);
      return Deque<T>::deque_.rend();
   }
   
   const_iterator begin() const {
      lock lk(mutex_);
      return Deque<T>::deque_.begin();
   }
   
   const_iterator end() const {
      lock lk(mutex_);
      return Deque<T>::deque_.end();
   }
   
   const_reverse_iterator rbegin() const {
      lock lk(mutex_);
      return Deque<T>::deque_.rbegin();
   }
   
   const_reverse_iterator rend() const {
      lock lk(mutex_);
      return Deque<T>::deque_.rend();
   }
   
   // accessors  =====================================================================
   size_t size() const {
      lock lk(mutex_);
      return queue_.size();
   }
   
   bool empty() const {
      lock lk(mutex_);
      return queue_.empty();
   }
   
   using Deque<T>::element;
   
   const T& front() {
      lock lk(mutex_);
      return queue_.top();
   }
   
   iterator elementDel(iterator it) {
      lock lk(mutex_);
      iterator ret = Deque<T>::elementDel(it);
      heapify();
      return ret;
   }
   
   void elementIs(uint32_t _i, const T& _e) {
      lock lk(mutex_);
      Deque<T>::elementIs(_i, _e);
      heapify();
   }
   
   void push(const T& _e) {
      lock lk(mutex_);
      queue_.push(_e);
   }
   
   void popFront() {
      lock lk(mutex_);
      queue_.pop();
   }
   
   void clear() {
      lock lk(mutex_);
      Deque<T>::clear();
      heapify();
   }
   
protected:
private:
   void heapify() {
      lock lk(mutex_);
      std::make_heap(Deque<T>::begin(), Deque<T>::end(), Compare());
   }
   // member functions ===============================================================
   // data members ===================================================================
   priority_queue<T,deque<T>,Compare> queue_;
   mutable boost::recursive_mutex mutex_;
};

} /* end of namespace Simone */
