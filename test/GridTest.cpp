#include <boost/test/unit_test.hpp>
#include "Grid.h"
#include "GridTools.h"
#include "Polyomino.h"
#include <vector>

using namespace TetrisAI;

std::vector<Polyomino> getTriominos() {
	Polyomino p1({ 3, 1 }), p2({ 7 }); // Create the two possible triominos (a L and a I)
	return std::vector<Polyomino>({ p1, p2 });
}

BOOST_AUTO_TEST_CASE(grid_constructors_test) {
	// Testing basic initialization
	Grid g(10, 20);
	BOOST_CHECK_EQUAL(20, g.getHeight());
	BOOST_CHECK_EQUAL(10, g.getWidth());
	BOOST_CHECK_EQUAL(20, g.getContent().size());
	BOOST_CHECK_EQUAL(0, g.getContent()[0]);
	BOOST_CHECK_EQUAL(0, g.getContent()[19]);

	// Sizes should be between 4 and 32 for both height and width
	BOOST_CHECK_THROW(Grid(4, 1), std::invalid_argument);
	BOOST_CHECK_THROW(Grid(1, 4), std::invalid_argument);
	BOOST_CHECK_THROW(Grid(4, 33), std::invalid_argument);
	BOOST_CHECK_THROW(Grid(33, 4), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(grid_fitPiece_test) {
	Grid g(6, 6); // Create a small grid 6x6
	std::vector<Polyomino> triominos(getTriominos());
	std::vector<unsigned int> gridContent, expectedContent;
	MoveResult lastMove, expectedMove;

	g.fitPiece(triominos[0], Transformation(0, 0));
	lastMove = g.fitPiece(triominos[1], Transformation(2, 1));
	expectedMove = MoveResult(false, 0, 0, 0);
	/* Grid last three rows [expected]:
	2.	---x--
	1.	---x-x
	0.	---xxx
	*/
	expectedContent = std::vector<unsigned int>({ 7, 5, 4, 0, 0, 0 });
	gridContent = g.getContent();
	BOOST_CHECK_EQUAL_COLLECTIONS(expectedContent.begin(), expectedContent.end(), gridContent.begin(), gridContent.end());
	BOOST_CHECK(lastMove == expectedMove);
	BOOST_CHECK_EQUAL(3, g.getTopHeight());

	lastMove = g.fitPiece(triominos[1], Transformation(3, 0));
	expectedMove = MoveResult(false, 1, 3, 0);
	/* Grid last three rows [expected]:
	2.	------
	1.	---x--
	0.	---x-x
	*/
	expectedContent = std::vector<unsigned int>({ 5, 4, 0, 0, 0, 0 });
	gridContent = g.getContent();
	BOOST_CHECK_EQUAL_COLLECTIONS(expectedContent.begin(), expectedContent.end(), gridContent.begin(), gridContent.end());
	BOOST_CHECK(lastMove == expectedMove);
	BOOST_CHECK_EQUAL(2, g.getTopHeight());

	lastMove = g.fitPiece(triominos[0], Transformation(4, 2));
	expectedMove = MoveResult(false, 0, 0, 0);
	/* Grid last three rows [expected]:
	2.	------
	1.	xx-x--
	0.	x--x-x
	*/
	expectedContent = std::vector<unsigned int>({ 37, 52, 0, 0, 0, 0 });
	gridContent = g.getContent();
	BOOST_CHECK_EQUAL_COLLECTIONS(expectedContent.begin(), expectedContent.end(), gridContent.begin(), gridContent.end());
	BOOST_CHECK(lastMove == expectedMove);
	BOOST_CHECK_EQUAL(2, g.getTopHeight());

	expectedMove = MoveResult(false, 0, 0, 2);
	lastMove = g.fitPiece(triominos[1], Transformation(5, 1));
	/* Grid expected content:
	5.	------ // Final row
	4.	x-----
	3.	x-----
	2.	x-----
	1.	xx-x--
	0.	x--x-x
	*/
	expectedContent = std::vector<unsigned int>({ 37, 52, 32, 32, 32, 0 });
	gridContent = g.getContent();
	BOOST_CHECK_EQUAL_COLLECTIONS(expectedContent.begin(), expectedContent.end(), gridContent.begin(), gridContent.end());
	BOOST_CHECK(lastMove == expectedMove);
	BOOST_CHECK_EQUAL(5, g.getTopHeight());
	
	// TRIGGER GAME OVER
	lastMove = g.fitPiece(triominos[0], Transformation(4, 0));
	expectedMove = MoveResult(true, 0, 0, -1);
	/* Grid expected content:
	X.	OxOOOO // The block does not fully fit
	5.	xx---- // Final row
	4.	x-----
	3.	x-----
	2.	x-----
	1.	xx-x--
	0.	x--x-x
	*/
	BOOST_CHECK(lastMove == expectedMove);

	// TESTING INVALID MOVE
	g = Grid(4, 4);
	BOOST_CHECK_THROW(g.fitPiece(triominos[0], Transformation(4, 0)), std::invalid_argument);
	BOOST_CHECK_THROW(g.fitPiece(triominos[0], Transformation(6, 1)), std::invalid_argument);
	BOOST_CHECK_THROW(g.fitPiece(triominos[1], Transformation(5, 0)), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(grid_columnTransitions_test) {
	Grid g(6, 6); // Small 6x6 grid
	std::vector<Polyomino> triominos(getTriominos());

	// Empty grid > transitions from empty to full at the bottom in each column (row "-1" is considered full)
	BOOST_CHECK_EQUAL(6, g.columnTransitions());

	g.fitPiece(triominos[0], Transformation(0, 0));
	g.fitPiece(triominos[1], Transformation(0, 0));
	/* Grid last three rows [expected]:
	2.	---xxx
	1.	-----x
	0.	----xx
	*/
	BOOST_CHECK_EQUAL(10, g.columnTransitions());

	g.fitPiece(triominos[0], Transformation(4, 2));
	/* Grid last three rows [expected]:
	2.	---xxx
	1.	xx---x
	0.	x---xx
	*/
	BOOST_CHECK_EQUAL(12, g.columnTransitions());
}