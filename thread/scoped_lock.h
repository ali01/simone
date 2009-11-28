#pragma once

#include <boost/thread/recursive_mutex.hpp>

#include "lock.h"
#include "recursive_mutex.h"

namespace Simone {
namespace thread {

class ScopedLock : public lock<boost::recursive_mutex::scoped_lock> {
public:
   explicit ScopedLock(RecursiveMutex& _l) : scoped_lock_(_l.boost_lockable()) {}
   boost::recursive_mutex::scoped_lock& boost_lock() { return scoped_lock_; }
private:
   boost::recursive_mutex::scoped_lock scoped_lock_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
