#ifndef SET_H_31X1QINF
#define SET_H_31X1QINF

#include <set>
#include <functional>

#include <boost/thread/recursive_mutex.hpp>

#include "utility.h"

#include "ptr.h"

namespace Simone {
using std::set;
using std::less;
using std::allocator;

template <typename T,
          bool _thread_safe_=false,
          typename Compare=less<T>,
          typename Allocator=allocator<T> >
class Set : public PtrInterface<Set<T,_thread_safe_,Compare,Allocator> >,
            private boost::noncopyable {
protected:
   typedef boost::recursive_mutex::scoped_lock lock;
   mutable boost::recursive_mutex mutex_;
public: // todo: support boost_foreach
   // type declarations ==============================================================
   typedef Simone::Ptr<const Set<T,_thread_safe_,Compare,Allocator> > PtrConst;
   typedef Simone::Ptr<Set<T,_thread_safe_,Compare,Allocator> > Ptr;
   
   typedef typename set<T,Compare,Allocator>::iterator               iterator;
   typedef typename set<T,Compare,Allocator>::reverse_iterator       reverse_iterator;
   typedef typename set<T,Compare,Allocator>::const_iterator         const_iterator;
   typedef typename set<T,Compare,Allocator>::const_reverse_iterator
                                                               const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr SetNew() { return new Set(); }
   Set() {} // allow static allocation
   ~Set() { if (_thread_safe_) { lock lk(mutex_); } }
   
   // iterators ======================================================================
   iterator begin() {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.begin();  
   }

   iterator end() {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.end();    
   }

   reverse_iterator rbegin() {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.rbegin(); 
   }

   reverse_iterator rend() {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.rend();   
   }

   
   const_iterator begin() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.begin();  
   }

   const_iterator end() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.end();    
   }

   const_reverse_iterator rbegin() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.rbegin(); 
   }

   const_reverse_iterator rend() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.rend();   
   }

   
   // accessors  =====================================================================
   size_t size() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.size();  
   }

   bool empty() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.empty(); 
   }

   
   iterator element(const T& _v) {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.find(_v); 
   }

   const_iterator element(const T& _v) const {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.find(_v); 
   }

   
   size_t count(const T& _v) const {
      if (_thread_safe_) { lock lk(mutex_); }
      return set_.count(_v); 
   }

   
   // mutators =======================================================================
   void elementIs(const T& _v) {
      if (_thread_safe_) { lock lk(mutex_); }
       set_.insert(_v); 
   }

   void elementDel(const T& _v) {
      if (_thread_safe_) { lock lk(mutex_); }
       set_.erase(_v); 
   }
private:
   // member functions ===============================================================
   // data members ===================================================================
   set<T,Compare,Allocator> set_;
};

} /* end of namespace Simone */

#endif
