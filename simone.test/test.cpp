#define BOOST_TEST_MODULE simone_test
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(basic) {
   BOOST_CHECK(true);
}

#include "../thread/thread.test/thread_test.h"