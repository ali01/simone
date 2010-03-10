/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once

#include <cstdlib>
#include <cassert>
#include <iomanip>
#include <cstring>
#include <utility>

#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#ifdef linux
#include <stdint.h>
#endif

#ifdef __APPLE__
#include <stdint.h>
#endif

#ifdef WIN32
#include "portability/vs-stdint.h"
#endif

#ifdef __DEBUG__ 
   #define IF_DEBUG(stmt)       stmt
   #define ABORT()              assert(false); abort()
   #define CONDITIONAL_DEBUG(condition, stmt, fl, ln)                      \
            if (condition) {                                               \
               printf("\nDebug segment @ %s:%d\n", fl, ln);                \
               printf("----------------------------------------------\n"); \
               stmt;                                                       \
               printf("\n\n");                                             \
            }
   #include <iostream>
   #include <sstream>
   #include <fstream>
   #include <typeinfo>
   #include <string>
   using std::cout;
   using std::endl;
   using std::cerr;
   using std::string;
   using std::ostream;
   using std::boolalpha;
   using std::stringstream;
   using std::ostringstream;
   using std::istringstream;
   
   #include <boost/thread/recursive_mutex.hpp>
   extern boost::recursive_mutex io_debug_mutex_;
#else   
   #define IF_DEBUG(stmt)
   #define ABORT()
   #define CONDITIONAL_DEBUG(condition, stmt, fl, ln)
   // #define NDEBUG
#endif

namespace Simone {} /* end of namespace Simone */
