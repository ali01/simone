/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once

#include <boost/thread/recursive_mutex.hpp>

#include "../utility.h"
#include "../exception.h"
#include "lock.h"

namespace Simone {
namespace thread {
/* forward declaration */ class RecursiveMutex;
class ScopedLock : public lock<boost::recursive_mutex::scoped_lock>,
                   private boost::noncopyable {
public:
   explicit ScopedLock(RecursiveMutex& _l);
   virtual ~ScopedLock();
   
   virtual void lock();
   virtual void unlock();
protected:
   RecursiveMutex *const mutex_;
   bool owns_lock_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
