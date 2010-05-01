#pragma once
#include "../inet.h"
#include "../../test.h"

#include "network_test_setup.h"

using namespace Simone;

namespace SimoneTest {

FIXTURE_TEST_SUITE(network, InetFixture);

TEST_CASE(_1) {
   AddressIPv4 addr(0xE0000009);
   CK_EQUAL(addr.str(), "224.0.0.9");
}

SUITE_END();

} /* end of namespace SimoneTest */
