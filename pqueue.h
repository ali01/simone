#pragma once
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>

#include "ptr.h"
#include "deque.h"
#include "utility.h"

using std::priority_queue;
using std::deque;
using std::less;

namespace Simone {
// TODO: support other operations
template <typename T, typename Compare=less<typename deque<T>::value_type> >
class PriorityQueue : private Deque<T> {
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const PriorityQueue<T,Compare> > PtrConst;
   typedef Simone::Ptr<PriorityQueue<T,Compare> > Ptr;
   
   typedef typename deque<T>::iterator               iterator;
   typedef typename deque<T>::reverse_iterator       reverse_iterator;
   typedef typename deque<T>::const_iterator         const_iterator;
   typedef typename deque<T>::const_reverse_iterator const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr PriorityQueueNew() { return new PriorityQueue(); }
   PriorityQueue() : queue_(Compare(), Deque<T>::deque_) {}
   
   // iterators ======================================================================
   iterator               begin()  { return Deque<T>::deque_.begin();  }
   iterator               end()    { return Deque<T>::deque_.end();    }
   reverse_iterator       rbegin() { return Deque<T>::deque_.rbegin(); }
   reverse_iterator       rend()   { return Deque<T>::deque_.rend();   }
   
   const_iterator         begin()  const { return Deque<T>::deque_.begin();  }
   const_iterator         end()    const { return Deque<T>::deque_.end();    }
   const_reverse_iterator rbegin() const { return Deque<T>::deque_.rbegin(); }
   const_reverse_iterator rend()   const { return Deque<T>::deque_.rend();   }
   
   // accessors  =====================================================================
   size_t size() const { return queue_.size(); }
   bool   empty() const { return queue_.empty(); }
   
   using Deque<T>::element;
   
   const T& front() { return queue_.top(); }
   
   iterator elementDel(iterator it) {
      iterator ret = Deque<T>::elementDel(it);
      heapify();
      return ret;
   }
   
   void elementIs(uint32_t _i, const T& _e) {
      Deque<T>::elementIs(_i, _e);
      heapify();
   }
   
   void pushFront(const T& _e) { queue_.push(_e); }
   void popFront() { queue_.pop(); }
   void clear() {
      Deque<T>::clear();
      heapify();
   }
protected:
private:
   void heapify() {
      std::make_heap(Deque<T>::begin(), Deque<T>::end(), Compare());
   }
   // member functions ===============================================================
   // data members ===================================================================
   priority_queue<T,deque<T>,Compare> queue_;
};

} /* end of namespace Simone */
