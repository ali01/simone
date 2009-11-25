#pragma once

#include <cstdlib>
#include <cassert>
#include <iomanip>
#include <cstring>

#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#ifdef __DEBUG__ 
   #define IF_DEBUG(stmt)       stmt
   #define ABORT()              assert(false); abort()
   #define DEBUG_IF(condition, stmt, fl, ln)                               \
            if (condition) {                                               \
               printf("\nDebug segment @ %s:%d\n", fl, ln);                \
               printf("----------------------------------------------\n"); \
               stmt;                                                       \
               printf("\n\n");                                             \
            }
   #include <iostream>
   #include <sstream>
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
#else   
   #define IF_DEBUG(stmt)
   #define ABORT()
   #define DEBUG_IF(condition, stmt, fl, ln)
   #define NDEBUG
#endif

namespace Simone {} /* end of namespace Simone */
