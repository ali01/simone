#pragma once
#include <boost/thread/thread.hpp>
#include "../time.h"

namespace Simone {

struct this_thread {
   static void sleep(const TimeDelta& _t) {
      boost::this_thread::sleep(_t.delta_);
   }
   
   static boost::thread::id id() { return boost::this_thread::get_id(); }
};

} /* end of namespace Simone */

