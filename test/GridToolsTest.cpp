#include <boost/test/unit_test.hpp>
#include "GridTools.h"

using namespace TetrisAI;

BOOST_AUTO_TEST_CASE(active_bits_count_test) {
	BOOST_CHECK_EQUAL(activeBitsCount(0), 0);
	BOOST_CHECK_EQUAL(activeBitsCount(1), 1);
	BOOST_CHECK_EQUAL(activeBitsCount(7), 3);
	BOOST_CHECK_EQUAL(activeBitsCount(4294967295), 32);
	BOOST_CHECK_EQUAL(activeBitsCount(17), 2);
}