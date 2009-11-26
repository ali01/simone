#include "thread_test_setup.h"

namespace SimoneTest {

void TestReactor::onStatus() {
   switch (test_mode_) {
      case kA:
         this_thread::sleep(seconds(2));
         answer_ = 42;
         break;
      case kB:
      case kC:
      case kD:
      default: return;
   }
}

} /* end of namespace SimoneTest */
