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
		/// <exception cred="std::invalid_argument">Thrown if the position of the polyomino is not valid</exception>
		/// <returns>Result status of the fall</returns>
		MoveResult fitPiece(const Polyomino& polyomino, Transformation transformation);

		// Getters
		const std::vector<unsigned int>& getContent() const;
		int getWidth() const;
		int getHeight() const;

		/// <summary>Returns the height of the the highest non-empty row (e.g returns 5 if row indexed at 4 is not empty and all rows above it are)</summary>
		int getTopHeight() const;

		// Evaluation utilities
		/// <summary>Returns the value of a full line (e.g. 1023 if the width's value is 10)</summary>
		unsigned int getCompleteLine() const;

		/// <summary>Returns the number of transition from 1 to 0 and 0 to 1 in columns in the grid</summary>
		int columnTransitions() const;

		/// <summary>Returns the number of transition from 1 to 0 and 0 to 1 in rows in the grid</summary>
		int rowTransitions() const;

		/// <summary>Returns the number of bits set to 0 that have a bit set to 1 in the same column above them</summary>
		int cellars() const;

		/// <summary>Returns a value relative to the number of wells in the grid. Each well is weighted depending on its depth</summary>
		/// <remarks>
		/// A well is an empty square that is surrounded by full squares on its left and right simultaneously 
		/// (x-x can be the top of a well, x--x can't) and all empty blocks below that square
		///
		/// The output value of a well is weighted by its depth. Thus, a 2-square deep well will count as 2, and a 4-square deep will count as 4
		/// The search for wells is perfomed for each square of the grid, thus, a square can be counted as part of multiple wells at once
		/// Example 1:
		///		2. ---xx-x
		///		1. ---xx-x
		///		0. ---xxxx
		///
		///		Here, we have a 2-square deep well at row 2 and a 1-square deep well at row 1. The second well is included in the first one but
		///		will be counted as well, thus outputing the value 3.
		///		This example shows the case of a "perfect" well of depth n. In such case, the sum of all sub wells will output the value n(n+1)/2 (the sum of i for i in [|0;n|])
		///
		/// Example 2:
		///		2. ---xx-x
		///		1. ---xx--
		///		0. ---xx-x
		///
		///		Here, we do not have a "perfect" well. At row 1, the pattern is not respected. Thus, we will obtain 3 for the well detected on row 2 and 1 for the
		///		well detected on row 0. Here we would output the value 4 instead of 6 for a "perfect" well with a 3-square depth.
		/// </remarks>
		int wells() const;

		/// <summary>Returns the number of blocks set to 0 consecutively in a column starting from a certain row</summary>
		/// <param name="row">Row at which the count is started</param>
		/// <param name="col">Column in which the count is performed</param>
		int emptyBlocksDown(int row, int col) const;

	private:
		std::vector<unsigned int> content;
		short width;
		short topHeight;

		/// <summary>Returns -1 if it can not fit. Else it returns the index of the lowest row where the piece would fit</summary>
		/// <param name="piece">Content of the piece that has to enter the grid</param>
		int findFittingRow(const std::vector<unsigned int>& piece) const;

		/// <summary>Returns false if some parts of the piece are out of the grid in terms of width</summary>
		/// <param name="piece">Content of the piece that must be tested</param>
		bool isPieceTranslationValid(const std::vector<unsigned int>& piece) const;

		/// <summary>Delete one row and translate down all rows above</summary>
		/// <param name="index">Row that must be deleted</param>
		void removeRow(unsigned int index);
	};

}

#endif
