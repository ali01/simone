#pragma once
#include <cstdlib>
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::boolalpha;

#define FIXTURE_TEST_SUITE BOOST_FIXTURE_TEST_SUITE
// #define TEST_SUITE         BOOST_TEST_SUITE
#define SUITE_END          BOOST_AUTO_TEST_SUITE_END
#define TEST_CASE          BOOST_AUTO_TEST_CASE
#define CK                 BOOST_CHECK
#define CK_EQUAL           BOOST_CHECK_EQUAL
#define CK_SMALL           BOOST_CHECK_SMALL
#define CK_GE              BOOST_CHECK_GE
#define CK_GT              BOOST_CHECK_GT
#define CK_LE              BOOST_CHECK_LE
#define CK_LT              BOOST_CHECK_LT
#define CK_NE              BOOST_CHECK_NE

namespace Simone {} /* end of namespace Simone */
