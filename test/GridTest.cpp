#include <boost/test/unit_test.hpp>
#include "Grid.h"
#include "GridTools.h"
#include "Polyomino.h"

using namespace TetrisAI;

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
	Polyomino p1({ 3, 1 }), p2({ 7 }); // Create two triominos (a L and a I)

	g.fitPiece(p1, Transformation(0, 0));
	g.fitPiece(p2, Transformation(2, 1));
	BOOST_CHECK_EQUAL(7, g.getContent()[0]);
	BOOST_CHECK_EQUAL(5, g.getContent()[1]);
	BOOST_CHECK_EQUAL(4, g.getContent()[2]);

	MoveResult result = g.fitPiece(p2, Transformation(3, 0));
	BOOST_CHECK_EQUAL(5, g.getContent()[0]);
	BOOST_CHECK_EQUAL(4, g.getContent()[1]);
	BOOST_CHECK_EQUAL(0, g.getContent()[2]);
	BOOST_CHECK_EQUAL(result.linesCleared, 1);
}