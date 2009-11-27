#ifndef MAP_H_LRU4H5ZH
#define MAP_H_LRU4H5ZH

#include <map>

#include <boost/thread/recursive_mutex.hpp>

#include "ptr.h"
#include "utility.h"


namespace Simone {
using std::map;

template <typename KeyType, typename ValueType, bool _thread_safe_=false>
class Map : public PtrInterface<Map<KeyType,ValueType,_thread_safe_> >,
            private boost::noncopyable {
protected:
   typedef boost::recursive_mutex::scoped_lock lock;
   mutable boost::recursive_mutex mutex_;
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const Map<KeyType,ValueType,_thread_safe_> > PtrConst;
   typedef Simone::Ptr<Map<KeyType,ValueType,_thread_safe_> > Ptr;
   
   typedef typename map<KeyType,ValueType>::iterator iterator;
   typedef typename map<KeyType,ValueType>::reverse_iterator 
                                                          reverse_iterator;
   typedef typename map<KeyType,ValueType>::const_iterator
                                                          const_iterator;
   typedef typename map<KeyType,ValueType>::const_reverse_iterator
                                                          const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr MapNew() { return new Map(); }
   Map() {}
   ~Map() { if (_thread_safe_) { lock lk(mutex_); } }
   
   // iterators ======================================================================
   iterator begin() {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.begin();
   }
   
   iterator end() {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.end();
   }
   
   reverse_iterator rbegin() {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.rbegin();
   }
   
   reverse_iterator rend() {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.rend();
   }
   
   const_iterator begin() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.begin();
   }
   
   const_iterator end() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.end();
   }
   
   const_reverse_iterator rbegin() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.rbegin();
   }
   
   const_reverse_iterator rend()   const {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.rend();
   }
   
   // accessors  =====================================================================
   size_t size()  const {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.size();
   }
   
   bool empty() const {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.empty();
   }
   
   iterator element(const KeyType& _key) {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.find(_key);
   }
   const_iterator element(const KeyType& _key) const {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_.find(_key);
   }
   
   // mutators =======================================================================
   void elementIs(const KeyType& _key, const ValueType& _v) {
      if (_thread_safe_) { lock lk(mutex_); }
      map_[_key] = _v;
   }
   
   void elementDel(iterator _it) {
      if (_thread_safe_) { lock lk(mutex_); }
      map_.erase(_it);
   }
   void elementDel(const KeyType& _key) {
      if (_thread_safe_) { lock lk(mutex_); }
      map_.erase(_key);
   }
   
   ValueType& operator[](const KeyType& key) {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_[key];
   }
   
   const ValueType& operator[](const KeyType& key) const {
      if (_thread_safe_) { lock lk(mutex_); }
      return map_[key];
   }
private:
   // member functions ===============================================================
   // data members ===================================================================
   map<KeyType,ValueType> map_;
};

} /* end of namespace Simone */

#endif
