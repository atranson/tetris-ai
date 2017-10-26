#include "Polyomino.h"
#include <stdexcept>

namespace TetrisAI {

	std::vector<Polyomino> Polyomino::getPolyominosList(unsigned int squares)
	{
		std::vector<Polyomino> list;
		switch (squares) {
			case 1:
				list.reserve(1);
				list.push_back(Polyomino({ 1 }));
				break;

			case 2:
				list.reserve(1);
				list.push_back(Polyomino({ 3 }));
				break;

			case 3:
				list.reserve(2);
				list.push_back(Polyomino({ 7 }));
				list.push_back(Polyomino({ 3, 1 }));
				break;

			case 4:
				list.reserve(7);
				list.push_back(Polyomino({ 2, 7 })); // T
				list.push_back(Polyomino({ 15 })); // I
				list.push_back(Polyomino({ 4, 7 })); // L
				list.push_back(Polyomino({ 1, 7 })); // J
				list.push_back(Polyomino({ 3, 3 })); // O
				list.push_back(Polyomino({ 6, 3 })); // S
				list.push_back(Polyomino({ 3, 6 })); // Z
				break;

			case 5:
				// See https://upload.wikimedia.org/wikipedia/commons/thumb/a/aa/All_18_Pentominoes.svg/850px-All_18_Pentominoes.svg.png
				// Elements created below are given in the same order as in the picture from left to right and top to bottom
				list.reserve(18);
				list.push_back(Polyomino({ 31 }));
				list.push_back(Polyomino({ 2, 6, 3 }));
				list.push_back(Polyomino({ 2, 3, 6 }));
				list.push_back(Polyomino({ 3, 1, 1, 1 }));
				list.push_back(Polyomino({ 3, 2, 2, 2 }));
				list.push_back(Polyomino({ 1, 3, 3, 3 }));
				list.push_back(Polyomino({ 2, 3, 3, 3 }));
				list.push_back(Polyomino({ 2, 3, 1, 1 }));
				list.push_back(Polyomino({ 1, 3, 2, 2 }));
				list.push_back(Polyomino({ 2, 2, 7 }));
				list.push_back(Polyomino({ 7, 5 }));
				list.push_back(Polyomino({ 7, 1, 1 }));
				list.push_back(Polyomino({ 6, 3, 1 }));
				list.push_back(Polyomino({ 2, 7, 2 }));
				list.push_back(Polyomino({ 1, 1, 3, 1 }));
				list.push_back(Polyomino({ 2, 2, 3, 2 }));
				list.push_back(Polyomino({ 6, 2, 3 }));
				list.push_back(Polyomino({ 3, 2, 6 }));
				break;

			default:
				throw std::invalid_argument("Unhandled case: can't generated polyomino list for a number of square greater than 4.");
		}

		return list;		
	}

	Polyomino::Polyomino(std::vector<unsigned int> baseContent)
	{
		// Stores the base state
		rotatedPieces.push_back(PolyominoState(baseContent));

		// Rotates the base state until we find it again
		int rotations(0);
		PolyominoState rotatedState(rotatedPieces[0].getRotatedState());
		while (!(rotatedState == rotatedPieces[0]) && rotations < 4)
		{
			rotatedPieces.push_back(rotatedState);
			rotatedState = rotatedState.getRotatedState();
			rotations++;
		}

		if (!(rotatedState == rotatedPieces[0]))
		{
			throw std::invalid_argument("An error occurred while generating rotated versions of the polyomino.");
		}
	}

	const PolyominoState& Polyomino::getRotatedPiece(int rotation) const
	{
		return rotatedPieces[rotation % rotatedPieces.size()];
	}

	const std::vector<unsigned int> Polyomino::getTransformedPiece(Transformation t) const
	{
		return getRotatedPiece(t.rotation).getTranslatedState(t.translation);
	}

	int Polyomino::getRotationCount() const
	{
		return rotatedPieces.size();
	}

}