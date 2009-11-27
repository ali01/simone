#pragma once

#include <deque>
using std::deque;

#include <boost/thread/recursive_mutex.hpp>

#include "ptr.h"
#include "utility.h"

namespace Simone {

template <typename T, bool _thread_safe_=false>
class Deque : public PtrInterface<Deque<T,_thread_safe_> >,
              private boost::noncopyable {
protected:
   deque<T> deque_;
   
   typedef boost::recursive_mutex::scoped_lock lock;
   mutable boost::recursive_mutex mutex_;
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const Deque<T,_thread_safe_> > PtrConst;
   typedef Simone::Ptr<Deque<T,_thread_safe_> > Ptr;
   
   typedef typename deque<T>::iterator               iterator;
   typedef typename deque<T>::reverse_iterator       reverse_iterator;
   typedef typename deque<T>::const_iterator         const_iterator;
   typedef typename deque<T>::const_reverse_iterator
                                                               const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr DequeNew() { return new Deque(); }
   Deque() {}
   ~Deque() { if (_thread_safe_) { lock lk(mutex_); } }
   
   // iterators ======================================================================
   iterator begin()  {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.begin();  
   }

   iterator end()    {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.end();    
   }

   reverse_iterator rbegin() {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.rbegin(); 
   }

   reverse_iterator rend()   {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.rend();   
   }

   
   const_iterator begin()  const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.begin();  
   }

   const_iterator end()    const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.end();    
   }

   const_reverse_iterator rbegin() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.rbegin(); 
   }

   const_reverse_iterator rend()   const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.rend();   
   }

   
   // accessors  =====================================================================
   size_t size() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.size(); 
   }

   bool empty() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.empty(); 
   }

   T& element(uint32_t _i) {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_[_i]; 
   }

   T& front() {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.front(); 
   }

   T& back() {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.back(); 
   }

   
   const T& element(uint32_t _i) const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_[_i]; 
   }

   const T& front() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.front(); 
   }

   const T& back() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.back(); 
   }

   
   iterator elementDel(iterator it) {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_.erase(it); 
   }

   void elementIs(uint32_t _i, const T& _e) {
      if (_thread_safe_) { lock lk(mutex_); }
      deque_[_i] = _e; 
   }

   
   void pushFront(const T& _e) {
      if (_thread_safe_) { lock lk(mutex_); }
      deque_.push_front(_e); 
   }

   void pushBack(const T& _e) {
      if (_thread_safe_) { lock lk(mutex_); }
      deque_.push_back(_e); 
   }

   void popFront() {
      if (_thread_safe_) { lock lk(mutex_); }
      deque_.pop_front(); 
   }

   void popBack() {
      if (_thread_safe_) { lock lk(mutex_); }
      deque_.pop_back(); 
   }

   void clear() {
      if (_thread_safe_) { lock lk(mutex_); }
      deque_.clear(); 
   }

   
   T& operator[](const uint32_t& _i) {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_[_i]; 
   }

   const T& operator[](const uint32_t& _i) const {
      if (_thread_safe_) { lock lk(mutex_); }
      return deque_[_i]; 
   }

};

} /* end of namespace Simone */
