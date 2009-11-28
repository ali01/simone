#include "utility.h"

#ifdef __DEBUG__
#ifdef __THREADING__
boost::recursive_mutex io_mutex_;
#endif
#endif