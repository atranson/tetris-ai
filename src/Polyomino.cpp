#include "Polyomino.h"
#include <stdexcept>

namespace TetrisAI {

	Polyomino::Polyomino(std::vector<unsigned int> baseContent)
	{
		rotatedPieces.push_back(PolyominoState(baseContent));

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
		return rotatedPieces[t.rotation % rotatedPieces.size()].getTranslatedState(t.translation);
	}

	unsigned int Polyomino::getRotationCount() const
	{
		return rotatedPieces.size();
	}

}