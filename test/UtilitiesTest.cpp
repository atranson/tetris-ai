#include <boost/test/unit_test.hpp>
#include "Utilities.h"

using namespace TetrisAI;

BOOST_AUTO_TEST_CASE(active_bits_count_test) {
	BOOST_CHECK_EQUAL(activeBitsCount(0), 0);
	BOOST_CHECK_EQUAL(activeBitsCount(1), 1);
	BOOST_CHECK_EQUAL(activeBitsCount(7), 3);
	BOOST_CHECK_EQUAL(activeBitsCount(4294967295), 32);
	BOOST_CHECK_EQUAL(activeBitsCount(17), 2);
}

BOOST_AUTO_TEST_CASE(split_range_test) {
	std::vector<int> 
		subRanges(splitRange(0, 32, 8)), 
		expectedOutput({0, 4, 8, 12, 16, 20, 24, 28, 32});
	BOOST_CHECK_EQUAL_COLLECTIONS(subRanges.begin(), subRanges.end(), expectedOutput.begin(), expectedOutput.end());

	subRanges = splitRange(0, 30, 8);
	expectedOutput = std::vector<int>{0, 4, 8, 12, 16, 20, 24, 27, 30};
	BOOST_CHECK_EQUAL_COLLECTIONS(subRanges.begin(), subRanges.end(), expectedOutput.begin(), expectedOutput.end());

	subRanges = splitRange(-6, 13, 4);
	expectedOutput = std::vector<int>{ -6, -2, 1, 4, 7 };
	BOOST_CHECK_EQUAL_COLLECTIONS(subRanges.begin(), subRanges.end(), expectedOutput.begin(), expectedOutput.end());

	subRanges = splitRange(-15, 3, 8);
	expectedOutput = std::vector<int>{ -15, -14, -13 , -12 };
	BOOST_CHECK_EQUAL_COLLECTIONS(subRanges.begin(), subRanges.end(), expectedOutput.begin(), expectedOutput.end());
}