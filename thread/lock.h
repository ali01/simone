#pragma once

#include "../utility.h"

namespace Simone {
namespace thread {

template <typename BoostLockType>
class lock : boost::noncopyable {
protected:
   lock() {}
   virtual ~lock() {}
   
   virtual BoostLockType& boost_lock() = 0;
};

} /* end of namespace thread */
} /* end of namespace Simone */
