#ifndef TETRISAI_GRID_H
#define TETRISAI_GRID_H

#include <vector>
#include "Polyomino.h"
#include "MoveResult.h"

namespace TetrisAI {

	class Grid {

	public:
		const static int sizeLimit = 32;

		Grid(short w, short h);
		Grid(const Grid &original);

		/// <summary>Make the polyomino (rotated and translated) fall into the grid and update the grid accordingly</summary>
		/// <param name="polyomino">Polyomino that should be added to the grid</param>
		/// <param name="transformation">Transformation that should be applied to the polyomino</param>
		/// <returns>Result status of the fall</returns>
		MoveResult fitPiece(const Polyomino& polyomino, Transformation transformation);

		const std::vector<unsigned int>& getContent() const;
		int getWidth() const;
		int getHeight() const;
		int getTopHeight() const;

		// Evaluation utilities
		unsigned int getCompleteLine() const;
		int columnTransitions() const;
		int rowTransitions() const;
		int cellars() const;
		int wells() const;
		int emptyBlocksDown(int row, int col) const;

	private:
		std::vector<unsigned int> content;
		short width;
		short topHeight;

		int findFittingRow(const std::vector<unsigned int>& piece) const;
		void removeRow(unsigned int index);
	};

}

#endif
