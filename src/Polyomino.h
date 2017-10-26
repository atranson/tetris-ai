#ifndef TETRISAI_POLYOMINO_H
#define TETRISAI_POLYOMINO_H

#include <vector>
#include "PolyominoState.h"

namespace TetrisAI {

	struct Transformation {
		int translation;
		int rotation;

		Transformation() : translation(0), rotation(0) {}
		Transformation(int t, int r) : translation(t), rotation(r) {}
		Transformation(const Transformation & original) : translation(original.translation), rotation(original.rotation) {}
	};

	class Polyomino {
	public:
		const static int maxSquares = 5;

		/// <summary>Helper to get the list of all possible Polyominos composed of a certain number of square</summary>
		/// <param name="squares">Number of squares that should compose the polyominos</param>
		static std::vector<Polyomino> getPolyominosList(unsigned int squares);

		/// <summary>Instantiates a Polyomino based on one of its possible state</summary>
		/// <param name="baseContent">Values defining a base piece on which the Polyomino and its rotation can be built</param>
		Polyomino(std::vector<unsigned int> baseContent);

		/// <summary>Retrieve a substate of the Polyomino corresponding to a certain number of rotations</summary>
		const PolyominoState& getRotatedPiece(int rotation) const;

		/// <summary>Applies a transformation to the Polyomino and returns the resulting raw content</summary>
		const std::vector<unsigned int> getTransformedPiece(Transformation t) const;
		int getRotationCount() const;

	private:
		/// <summary>Stores all sub states of the polyomino: one for each possible rotation</summary>
		std::vector<PolyominoState> rotatedPieces;
	};
}

#endif
