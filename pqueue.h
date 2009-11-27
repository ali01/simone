#pragma once
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>

#include <boost/thread/recursive_mutex.hpp>

#include "ptr.h"
#include "deque.h"
#include "utility.h"

using std::priority_queue;
using std::deque;
using std::less;

namespace Simone {
// TODO: support other operations
template <typename T,
          typename Compare=less<typename deque<T>::value_type>,
          bool _thread_safe_=false>
class PriorityQueue : private Deque<T,_thread_safe_> {
protected:
   typedef boost::recursive_mutex::scoped_lock lock;
   mutable boost::recursive_mutex mutex_;
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const PriorityQueue<T,Compare,_thread_safe_> > PtrConst;
   typedef Simone::Ptr<PriorityQueue<T,Compare,_thread_safe_> > Ptr;
   
   typedef typename deque<T>::iterator               iterator;
   typedef typename deque<T>::reverse_iterator       reverse_iterator;
   typedef typename deque<T>::const_iterator         const_iterator;
   typedef typename deque<T>::const_reverse_iterator const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr PriorityQueueNew() { return new PriorityQueue(); }
   PriorityQueue() : queue_(Compare(), Deque<T,_thread_safe_>::deque_) {}
   
   // iterators ======================================================================
   iterator               begin()  { return Deque<T,_thread_safe_>::deque_.begin();  }
   iterator               end()    { return Deque<T,_thread_safe_>::deque_.end();    }
   reverse_iterator       rbegin() { return Deque<T,_thread_safe_>::deque_.rbegin(); }
   reverse_iterator       rend()   { return Deque<T,_thread_safe_>::deque_.rend();   }
   
   const_iterator         begin()  const { return Deque<T,_thread_safe_>::deque_.begin();  }
   const_iterator         end()    const { return Deque<T,_thread_safe_>::deque_.end();    }
   const_reverse_iterator rbegin() const { return Deque<T,_thread_safe_>::deque_.rbegin(); }
   const_reverse_iterator rend()   const { return Deque<T,_thread_safe_>::deque_.rend();   }
   
   // accessors  =====================================================================
   size_t size() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return queue_.size();
   }
   
   bool empty() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return queue_.empty();
   }
   
   using Deque<T,_thread_safe_>::element;
   
   const T& front() {
      if (_thread_safe_) { lock lk(mutex_); }
      return queue_.top();
   }
   
   iterator elementDel(iterator it) {
      iterator ret = Deque<T,_thread_safe_>::elementDel(it);
      heapify();
      return ret;
   }
   
   void elementIs(uint32_t _i, const T& _e) {
      Deque<T,_thread_safe_>::elementIs(_i, _e);
      heapify();
   }
   
   void push(const T& _e) {
      if (_thread_safe_) { lock lk(mutex_); }
      queue_.push(_e);
   }
   
   void pop() {
      if (_thread_safe_) { lock lk(mutex_); }
      queue_.pop();
   }
   
   void clear() {
      Deque<T,_thread_safe_>::clear();
      heapify();
   }
private:
   void heapify() {
      if (_thread_safe_) { lock lk(mutex_); }
      std::make_heap(Deque<T,_thread_safe_>::begin(),
                     Deque<T,_thread_safe_>::end(),
                     Compare());
   }
   // member functions ===============================================================
   // data members ===================================================================
   priority_queue<T,deque<T>,Compare> queue_;
};

} /* end of namespace Simone */
