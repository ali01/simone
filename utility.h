/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#ifndef UTILITY_H_209U9Q7M
#define UTILITY_H_209U9Q7M

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <utility>
#include <iomanip>

#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#if defined(linux) || defined(__APPLE__)
#include <stdint.h>
#endif

#ifdef WIN32
#include "portability/vs-stdint.h"
#endif

#define ABORT() assert(false);

#ifdef __DEBUG__
   #define IF_DEBUG(stmt)       stmt
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
#endif

namespace Simone {} /* end of namespace Simone */

#endif