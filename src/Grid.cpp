#include "Grid.h"
#include <stdexcept>
#include <algorithm>
#include "GridTools.h"

namespace TetrisAI {

	Grid::Grid(short w, short h) : width(w), topHeight(0)
	{
		if (w > Grid::maxSize || h > Grid::maxSize)
		{
			throw std::invalid_argument("A grid can't exceed 32 in width or 32 in height.");
		}
		if (w < Grid::minSize || h < Grid::minSize)
		{
			throw std::invalid_argument("A grid must be at least 4 blocks wide and 4 blocks high.");
		}

		content = std::vector<unsigned int>(h);
	}

	Grid::Grid(const Grid &original)
	{
		width = original.width;
		content = original.content;
		topHeight = original.topHeight;
	}

	MoveResult Grid::fitPiece(const Polyomino & polyomino, Transformation transformation)
	{
		// in the following, piece.size() represents the height of the piece as a PolyominoState is guaranteed to have all its rows non empty
		const std::vector<unsigned int> piece(polyomino.getTransformedPiece(transformation));
		
		if (!isPieceTranslationValid(piece))
		{
			throw std::invalid_argument("The piece translation makes it fall out of the grid");
		}
		
		MoveResult result;
		result.landingRow = findFittingRow(piece);

		// Could not integrate the piece in the grid
		if (result.landingRow == -1)
		{
			result.gameOver = true;
			return result;
		}

		int landingTopHeight(result.landingRow + piece.size());
		// If it is ok: we merge the piece and the grid and clear the complete lines along the way
		topHeight = (landingTopHeight > topHeight) ? landingTopHeight : topHeight;

		// Browse the rows of the grid that must change and clear complete lines if needed
		for (unsigned h = 0; h < piece.size(); h++)
		{
			int currentRow = result.landingRow + h - result.linesCleared;
			content[currentRow] = piece[h] | content[currentRow];

			if (content[currentRow] == getCompleteLine())
			{
				result.pieceVanishedBlocks += activeBitsCount(piece[h]);
				removeRow(currentRow);
				result.linesCleared++;
				topHeight--;
			}
		}

		return result;
	}

	
	int Grid::findFittingRow(const std::vector<unsigned int>& piece) const
	{
		int landingRow(topHeight);
		// If the grid is not empty, we go down until there is a collision
		if (landingRow != 0)
		{
			int collisionSum(0);
			while (collisionSum == 0 && landingRow > 0)
			{
				landingRow--;
				short rangeTop = std::min(getHeight(), landingRow + (int)piece.size());
				for (int h = landingRow; h < rangeTop; h++)
				{
					collisionSum += piece[h - landingRow] & content[h];
				}
			}

			// If we stopped due to a collision, we have to go one step up (the last height with no collision)
			landingRow = (collisionSum == 0) ? landingRow : landingRow + 1;
		}

		// THEN: we check if it's a game over situation, if it is we return -1, else we return the lowest row where the piece would fit
		return (landingRow + (int)(piece.size()) > getHeight()) ? -1 : landingRow;
	}

	bool Grid::isPieceTranslationValid(const std::vector<unsigned int>& piece) const
	{
		unsigned int maxValue(getCompleteLine()); // highest possible bit considering the width of the grid
		for (auto& rowValue : piece)
		{
			if (rowValue > maxValue)
			{
				return false;
			}
		}
		return true;
	}

	const std::vector<unsigned int>& Grid::getContent() const
	{
		return content;
	}

	unsigned int Grid::getCompleteLine() const
	{
		return (1 << getWidth()) - 1;
	}

	void Grid::removeRow(unsigned int row)
	{
		for (int index = row; index < getHeight() - 1; index++)
		{
			content[index] = content[index + 1];
		}
		content[getHeight() - 1] = 0;
	}

	int Grid::getWidth() const
	{
		return width;
	}

	int Grid::getHeight() const
	{
		return content.size();
	}

	int Grid::getTopHeight() const
	{
		return topHeight;
	}

	int Grid::columnTransitions() const
	{
		int output(0);

		// Transition at the upper and lower boundaries of the non-empty grid
		if(topHeight > 0) 
		{
			output += activeBitsCount(content[getTopHeight() - 1] ^ 0); // Upper boundary of the grid
		}
		output += activeBitsCount(getCompleteLine() ^ content[0]); // Lower boundary of the grid

		// Transition inside the grid
		for (int row = 0; row < getTopHeight() - 1; row++)
		{
			output += activeBitsCount(content[row] ^ content[row + 1]);
		}

		return output;
	}

	int Grid::rowTransitions() const
	{
		int output(0), gridWidth(getWidth());

		unsigned int highestOrderBit(1 << (gridWidth - 1));

		for (int row = 0; row < getTopHeight(); row++)
		{
			// Transitions at the boundaries of the grid
			if (!(content[row] & highestOrderBit)) { output++; } // Left boundary of the grid (add 1 if the most left bit is 0)
			if (!(content[row] & 1)) { output++; } // Right boundary of the grid (add 1 if the most right bit is 0)

			// Transitions inside the grid
			for (int col = 0; col < gridWidth - 1; col++)
			{
				output += ((content[row] >> col) & 1) ^ ((content[row] >> (col + 1)) & 1);
			}
		}

		return output;
	}

	int Grid::cellars() const
	{
		int output(0), roofMask(0);
		unsigned int completeLine(getCompleteLine());

		for (int row = getTopHeight() - 1; row >= 0; row--)
		{
			// roofMask bits which are set to 1 indicates that, in the corresponding column, in a higher position, a bit is set to 1
			// Thus if there is a match, with completeLine - gridContent[row] that means, we found a hole
			roofMask = roofMask & (completeLine - content[row]); // completeLine - gridContent[row] is equivalent to performing ~gridContent[row] on the first n-bytes with n=width of the grid
			output += activeBitsCount(roofMask); // Count of the number of holes found
			roofMask = roofMask | content[row]; // New mask is the combination of found holes and full blocks in the current row
		}

		return output;
	}

	int Grid::wells() const
	{
		// wellMask allows to filter rows to keep only the bits of 3 columns (or 2 if we are next to the borders of the grid)
		// wellPattern indicates what should be the value of the filtering via wellMask if the pattern describing the top of a well is found
		//
		// Example 1.	with 3 and 2 as values, we can search for wells on the right side of the grid as it would match rows ending with 10
		// Example 2.	with 7 and 5 as values, we can search for wells in the second column as it would match rows ending with 101
		int output(0), wellMask(3), wellPattern(2), gridWidth(getWidth());

		for (int col = 0; col < gridWidth; col++)
		{
			for (int row = getTopHeight() - 1; row >= 0; row--)
			{
				// Check if the pattern is respected for the columns filtered by the mask
				if ((content[row] & wellMask) == wellPattern)
				{
					output += 1 + emptyBlocksDown(row - 1, col);
				}
			}

			// Modify the pattern and mask for the next column
			if (col == 0)
			{
				// If it was the first step, we have to change the values
				wellMask = 7;
				wellPattern = 5;
			}
			else if (col == gridWidth - 2)
			{
				// If we reached, the end of the grid, we have to change the values
				wellMask = (1 << (gridWidth - 1)) + (1 << (gridWidth - 2)); // e.g 512 + 256 if W=10
				wellPattern = 1 << (gridWidth - 2); // e.g 256 if W=10
			}
			else
			{
				// If we are going from one basic case to another, we just shift values
				wellMask <<= 1;
				wellPattern <<= 1;
			}
		}

		return output;
	}

	int Grid::emptyBlocksDown(int row, int col) const
	{
		int output(0);
		// While we have empty blocks in the column col, we keep going down
		while (row >= 0 && !((content[row] >> col) & 1))
		{
			output++;
			row--;
		}

		return output;
	}

}