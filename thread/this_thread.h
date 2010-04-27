/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once
#include <boost/thread/thread.hpp>
// #include "../time/time.h"

namespace Simone {

/* forward declaration */
class TimeDelta;

namespace thread {

struct this_thread {
   static void sleep(const TimeDelta& _t);
   static boost::thread::id id() { return boost::this_thread::get_id(); }
};

} /* end of namespace thread */
} /* end of namespace Simone */
