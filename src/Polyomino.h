#ifndef TETRISAI_POLYOMINO_H
#define TETRISAI_POLYOMINO_H

#include <vector>
#include "PolyominoState.h"

namespace TetrisAI {

	struct Transformation {
		int translation;
		int rotation;
	};

	class Polyomino {
	public:
		Polyomino(std::vector<unsigned int> baseContent);
		const PolyominoState& getRotatedPiece(int rotation) const;
		const std::vector<unsigned int> getTransformedPiece(Transformation t) const;
		unsigned int getRotationCount() const;

	private:
		std::vector<PolyominoState> rotatedPieces;
	};
}

#endif
