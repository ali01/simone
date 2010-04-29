/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once

#include <vector>
using std::vector;

#include <boost/thread/recursive_mutex.hpp>

#include "../utility.h"
#include "../exception.h"

namespace Simone {

// v0.1
class RecursiveMutex : boost::noncopyable {
   typedef boost::recursive_mutex recursive_mutex_t;
   typedef boost::recursive_mutex::scoped_lock scoped_lock_t;
public:
   RecursiveMutex() : lock_count_(0),
                      owns_mutex_(true),
                      mutex_(new recursive_mutex_t()) {}
   
   ~RecursiveMutex() {
      delete_mutex();
   }
   
   void lock() {
      scoped_lock_t lk(*mutex_);
      mutex_->lock();
      lock_count_++;
   }
   
   void unlock() {
      scoped_lock_t lk(*mutex_);
      mutex_->unlock();
      if (lockCount() == 0) {
         cerr << __FILE__ << ":" << __LINE__ << endl;
         stringstream ss;
         ss << "attempt to unlock a mutex with a lock count of zero.\n";
         ss << "Count: " << lockCount() << endl;
         throw InvalidOperationException(__FILE__, __LINE__, ss.str());
      }
      lock_count_--;
   }
   
   void parentMutexIs(const RecursiveMutex& o) {
      assert_combinable(o, *this);
      this->mutex_is(o);
   }
   
   void childMutexNew(RecursiveMutex& o) {
      assert_combinable(*this, o);
      o.mutex_is(*this);
   }
   
   uint32_t lockCount() const { 
      scoped_lock_t lk(*mutex_);
      return lock_count_;
   }

private: 
   void mutex_is(const RecursiveMutex& o) {
      delete_mutex();
      mutex_ = o.mutex_;
   }
   
   void delete_mutex() {
      mutex_->lock();
      if (owns_mutex_) {
         mutex_->unlock();
         delete mutex_;
         owns_mutex_ = false; // mutex cannot be reinitialized
      } else { mutex_->unlock(); }
      mutex_ = NULL;
   }
   
   void assert_combinable(const RecursiveMutex& parent, 
                          const RecursiveMutex& child) {
      scoped_lock_t lk(*mutex_);
      if (parent.lockCount() != 0 || child.lockCount() != 0) {
         cerr << __FILE__ << ":" << __LINE__ << endl;
         stringstream ss;
         ss << "attempt to combine mutexes with non-zero lock counts\n";
         ss << "Parent lock count: " << parent.lockCount() << endl;
         ss << "Child lock count:  " << child.lockCount() << endl;
         throw InvalidOperationException(__FILE__, __LINE__, ss.str());
      }
   }
   
   uint32_t lock_count_;
   bool owns_mutex_;
   mutable recursive_mutex_t *mutex_;
};

} /* end of namespace Simone */
