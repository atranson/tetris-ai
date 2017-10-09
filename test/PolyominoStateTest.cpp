#define BOOST_TEST_MODULE PolyominoState test
#include <boost/test/unit_test.hpp>
#include "PolyominoState.h"
#include <vector>

using namespace TetrisAI;

BOOST_AUTO_TEST_CASE(constructors_test) {
	std::vector<unsigned int> c0(std::vector<unsigned int> {2, 7});
	PolyominoState p0(c0);
	std::vector<unsigned int> p0Content(p0.getTranslatedState(0));
	BOOST_CHECK_EQUAL(p0.getHeight(), 2);
	BOOST_CHECK_EQUAL(p0.getWidth(), 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(c0.begin(), c0.end(), p0Content.begin(), p0Content.end());

	// Empty rows should be forbidden
	BOOST_CHECK_THROW(PolyominoState(std::vector<unsigned int> {1, 0}), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(translation_test) {
	std::vector<unsigned int> content(std::vector<unsigned int> {2, 7, 1});
	PolyominoState piece(content);

	std::vector<unsigned int> t1(piece.getTranslatedState(1)), t2(piece.getTranslatedState(2)), t3(piece.getTranslatedState(5));
	std::vector<unsigned int> expected1({ 4, 14, 2 }), expected2({ 8, 28, 4 }), expected3({ 64, 224, 32 });

	BOOST_CHECK_EQUAL_COLLECTIONS(t1.begin(), t1.end(), expected1.begin(), expected1.end());
	BOOST_CHECK_EQUAL_COLLECTIONS(t2.begin(), t2.end(), expected2.begin(), expected2.end());
	BOOST_CHECK_EQUAL_COLLECTIONS(t3.begin(), t3.end(), expected3.begin(), expected3.end());
}

BOOST_AUTO_TEST_CASE(rotation_test) {
	// Tests for each tetramino
	std::vector<std::vector<std::vector<unsigned int>>> tetraminos({
		{ {2, 7}, {1, 3, 1}, {7, 2}, {2, 3, 2} }, // T
		{ {15}, {1, 1, 1, 1} }, // I
		{ {4, 7}, {1, 1, 3}, {7, 1}, {3, 2, 2} }, // L
		{ {1, 7}, {3, 1, 1}, {7, 4}, {2, 2, 3} }, // J
		{ {3, 3} }, // O
		{ {6, 3}, {1, 3, 2} }, // S
		{ {3, 6}, {2, 3, 1} } // Z
	});

	for (auto& tetramino : tetraminos) {
		std::vector<std::vector<unsigned int>>::iterator it = tetramino.begin();
		PolyominoState base(*it);
		PolyominoState currentRotation(base);

		for (++it; it != tetramino.end(); ++it) {
			currentRotation = currentRotation.getRotatedState();
			BOOST_CHECK(PolyominoState(*it) == currentRotation);
		}

		// Last check to see if we end up on the same tetramino if we rotated all the way
		currentRotation = currentRotation.getRotatedState();
		BOOST_CHECK(base == currentRotation);
	}
}