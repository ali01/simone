#include "activity_manager.h"

namespace Simone {
namespace thread {

ActivityManager::Ptr ActivityManager::ActivityManagerNew() {
   return new ActivityManager();
}

} /* end of namespace thread */
} /* end of namespace Simone */
