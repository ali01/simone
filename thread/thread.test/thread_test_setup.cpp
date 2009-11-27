#include "thread_test_setup.h"
#include "../../test.h"

namespace SimoneTest {

void TestActivityTask::onRun() {
   lock lk(mutex_);
   switch (test_mode_) {
      case kA:
         answer_ = 42;
         break;
      case kB:
         answer_ = 2737;
         break;
      case kC:
         answer_ = 38193;
         break;
      case kD:
         answer_ = 29218;
         break;
      default: return;
   }
}

} /* end of namespace SimoneTest */
