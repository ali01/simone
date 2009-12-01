#pragma once

namespace Simone {
namespace thread {

template <typename BoostLockType>
class lock {
protected:
   lock() {}
   virtual ~lock() {}
};

} /* end of namespace thread */
} /* end of namespace Simone */
