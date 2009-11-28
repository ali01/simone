#pragma once

#include "lockable.h"

namespace Simone {
namespace thread {

template <typename BoostMutexType>
class Mutex : public lockable<Mutex<BoostMutexType>, BoostMutexType> {
protected:
   Mutex() {}
   virtual ~Mutex() {}
};

} /* end of namespace thread */
} /* end of namespace Simone */
