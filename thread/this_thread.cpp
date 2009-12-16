/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#include "this_thread.h"
#include "../time.h"

namespace Simone {
namespace thread {

void this_thread::sleep(const TimeDelta& _t) {
   boost::this_thread::sleep(_t.boost_time_duration());
}

} /* end of namespace thread */
} /* end of namespace Simone */
