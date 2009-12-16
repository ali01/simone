/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#include "activity_manager.h"

namespace Simone {
namespace thread {

ActivityManager::Ptr ActivityManager::ActivityManagerNew() {
   return new ActivityManager();
}

} /* end of namespace thread */
} /* end of namespace Simone */
