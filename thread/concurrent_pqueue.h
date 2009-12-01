#pragma once
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>

#include "recursive_mutex.h"
#include "scoped_lock.h"

#include "../ptr_interface.h"
#include "../utility.h"

#include "concurrent_deque.h"

using std::priority_queue;
using std::deque;
using std::less;

namespace Simone {
namespace thread {

// TODO: support other operations
template <typename T,
          typename Compare=less<typename deque<T>::value_type> >
class ConcurrentPriorityQueue : private ConcurrentDeque<T> {
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
   ConcurrentPriorityQueue() : queue_(Compare(), ConcurrentDeque<T>::deque_) {}
   virtual ~ConcurrentPriorityQueue() {
      ScopedLock lk(this->mutex());
   }
   
   // iterators ======================================================================
   iterator               begin()  { return ConcurrentDeque<T>::deque_.begin();  }
   iterator               end()    { return ConcurrentDeque<T>::deque_.end();    }
   reverse_iterator       rbegin() { return ConcurrentDeque<T>::deque_.rbegin(); }
   reverse_iterator       rend()   { return ConcurrentDeque<T>::deque_.rend();   }
   
   const_iterator         begin()  const { return ConcurrentDeque<T>::deque_.begin();  }
   const_iterator         end()    const { return ConcurrentDeque<T>::deque_.end();    }
   const_reverse_iterator rbegin() const { return ConcurrentDeque<T>::deque_.rbegin(); }
   const_reverse_iterator rend()   const { return ConcurrentDeque<T>::deque_.rend();   }
   
   // accessors  =====================================================================
   using ConcurrentDeque<T>::element;
   using ConcurrentDeque<T>::mutex;
   
   size_t size() const {
      ScopedLock lk(this->mutex());
      return queue_.size();
   }
   
   bool empty() const {
      ScopedLock lk(this->mutex());
      return queue_.empty();
   }
   
   const T& front() {
      ScopedLock lk(this->mutex());
      return queue_.top();
   }
   
   const T& front() const {
      ScopedLock lk(this->mutex());
      return queue_.top();
   }
   
   iterator elementDel(iterator it) {
      iterator ret = ConcurrentDeque<T>::elementDel(it);
      heapify();
      return ret;
   }
   
   void elementIs(uint32_t _i, const T& _e) {
      ConcurrentDeque<T>::elementIs(_i, _e);
      heapify();
   }
   
   void push(const T& _e) {
      ScopedLock lk(this->mutex());
      queue_.push(_e);
   }
   
   void pop() {
      // TODO: Think about sync issues. locking this-> mutex causes deadlock
      queue_.pop();
   }
   
   void clear() {
      ConcurrentDeque<T>::clear();
      heapify();
   }
private:
   void heapify() {
      ScopedLock lk(this->mutex());
      std::make_heap(ConcurrentDeque<T>::begin(),
                     ConcurrentDeque<T>::end(),
                     Compare());
   }
   // member functions ===============================================================
   // data members ===================================================================
   priority_queue<T,deque<T>,Compare> queue_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
