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
	/* Grid last three rows:
	2.	---xxx
	1.	-----x
	0.	----xx
	*/
	BOOST_CHECK_EQUAL(10, g.columnTransitions());

	g.fitPiece(triominos[0], Transformation(4, 2));
	/* Grid last three rows:
	2.	---xxx
	1.	xx---x
	0.	x---xx
	*/
	BOOST_CHECK_EQUAL(12, g.columnTransitions());
}

BOOST_AUTO_TEST_CASE(grid_getCompleteLine_test) {
	BOOST_CHECK_EQUAL(1023, Grid(10, 4).getCompleteLine());
	BOOST_CHECK_EQUAL(15, Grid(4, 4).getCompleteLine());
}

BOOST_AUTO_TEST_CASE(grid_rowTransitions_test) {
	Grid g(6, 6); // Small 6x6 grid
	std::vector<Polyomino> triominos(getTriominos());

	// Empty grid > no transitions since only non empty rows should be considered
	BOOST_CHECK_EQUAL(0, g.rowTransitions());

	g.fitPiece(triominos[0], Transformation(1, 0));
	g.fitPiece(triominos[1], Transformation(4, 1));
	/* Grid last three rows:
	2.	-x----
	1.	-x--x-
	0.	-x-xx-
	*/
	BOOST_CHECK_EQUAL(16, g.rowTransitions());

	g.fitPiece(triominos[1], Transformation(5, 1));
	/* Grid last three rows:
	2.	xx----
	1.	xx--x-
	0.	xx-xx-
	*/
	BOOST_CHECK_EQUAL(10, g.rowTransitions());
}

BOOST_AUTO_TEST_CASE(grid_cellars_test) {
	Grid g(6, 6); // Small 6x6 grid
	std::vector<Polyomino> triominos(getTriominos());

	// Empty grid > no cellars possible
	BOOST_CHECK_EQUAL(0, g.cellars());

	g.fitPiece(triominos[0], Transformation(1, 0));
	g.fitPiece(triominos[1], Transformation(0, 0));
	/* Grid last three rows:
	2.	---xxx
	1.	----x-
	0.	---xx-
	*/
	BOOST_CHECK_EQUAL(3, g.cellars());

	g.fitPiece(triominos[1], Transformation(2, 0));
	/* Grid last rows:
	3.	-xxx--
	2.	---xxx
	1.	----x-
	0.	---xx-
	*/
	BOOST_CHECK_EQUAL(9, g.cellars());
}

BOOST_AUTO_TEST_CASE(grid_wells_test) {
	Grid g(6, 6); // Small 6x6 grid
	std::vector<Polyomino> triominos(getTriominos());

	// Empty grid > no wells possible
	BOOST_CHECK_EQUAL(0, g.wells());

	g.fitPiece(triominos[0], Transformation(1, 0));
	g.fitPiece(triominos[1], Transformation(0, 0));
	/* Grid last three rows: (wells are marked with O)
	2.	---xxx
	1.	----xO
	0.	---xxO
	*/
	// A well of depth n will output the sum of i for i from 1 to n (thus n(n+1)/2)
	BOOST_CHECK_EQUAL(3, g.wells()); // Here, we have only one well of depth=2 > 2+1=2*3/2=3

	g.fitPiece(triominos[1], Transformation(4, 1));
	/* Grid last rows:
	2.	-x-xxx
	1.	-x--x- // The central zone is not a well (for column=4) as there is an open position on the right
	0.	-x-xx-
	*/
	// We have 3 wells, but the central one is not "complete" thus it will output 4 instead of 6 (3 + 1)
	// Thus, we expect (from left to right) 6 + 4 + 3
	BOOST_CHECK_EQUAL(13, g.wells());
}

BOOST_AUTO_TEST_CASE(grid_emptyBlocksDown_test) {
	Grid g(6, 6); // Small 6x6 grid
	std::vector<Polyomino> triominos(getTriominos());

	// Empty grid > if we start at row n, we will get n+1 as a return value for each column
	BOOST_CHECK_EQUAL(3, g.emptyBlocksDown(2, 2));
	BOOST_CHECK_EQUAL(5, g.emptyBlocksDown(4, 0));

	g.fitPiece(triominos[0], Transformation(1, 0));
	g.fitPiece(triominos[1], Transformation(0, 0));
	/* Grid last three rows:
	2.	---xxx
	1.	----x-
	0.	---xx-
	*/
	BOOST_CHECK_EQUAL(0, g.emptyBlocksDown(2, 0));
	BOOST_CHECK_EQUAL(2, g.emptyBlocksDown(1, 0));
	BOOST_CHECK_EQUAL(1, g.emptyBlocksDown(1, 2));
}