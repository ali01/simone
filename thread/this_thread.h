#pragma once
#include "../time.h"

namespace Simone {

struct this_thread {
   static void sleep(const TimeDelta& _t) {
      boost::this_thread::sleep(_t.delta_);
   }
};

} /* end of namespace Simone */

