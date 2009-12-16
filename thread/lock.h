/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

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
