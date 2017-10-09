#include <boost/test/unit_test.hpp>
#include "Polyomino.h"
#include <vector>

using namespace TetrisAI;

BOOST_AUTO_TEST_CASE(polyomino_constructors_test) {
	// Test with the tetramino T
	std::vector<unsigned int> 
		baseContent({ 2, 7 }),
		expectedR1({ 1, 3, 1 }),
		expectedR2({ 7, 2 }),
		expectedR3({ 2, 3, 2 });

	PolyominoState ps0(baseContent), ps1(expectedR1), ps2(expectedR2), ps3(expectedR3);
	Polyomino p(baseContent);
	PolyominoState 
		r0(p.getRotatedPiece(0)),
		r1(p.getRotatedPiece(1)),
		r2(p.getRotatedPiece(2)),
		r3(p.getRotatedPiece(3)),
		r4(p.getRotatedPiece(4));

	BOOST_CHECK_EQUAL(p.getRotationCount(), 4);
	BOOST_CHECK(r0 == ps0);
	BOOST_CHECK(r1 == ps1);
	BOOST_CHECK(r2 == ps2);
	BOOST_CHECK(r3 == ps3);
	BOOST_CHECK(r4 == ps0);
}