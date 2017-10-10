#include "Grid.h"
#include <stdexcept>
#include <algorithm>
#include "GridTools.h"

namespace TetrisAI {

	Grid::Grid(short w, short h) : width(w), topHeight(0)
	{
		if (w > 32 || h > 32)
		{
			throw std::invalid_argument("A grid can't exceed 32 in width or 32 in height.");
		}
		if (w < 4 || h < 4)
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
		MoveResult result;
		result.landingRow = findFittingRow(piece);

		if (result.landingRow == -1)
		{
			// Could not integrate the piece in the grid
			result.gameOver = true;
			return result;
		}

		int landingTopHeight(result.landingRow + piece.size());
		// If it is ok: we merge the piece and the grid and clear the complete lines along the way
		topHeight = (landingTopHeight > topHeight) ? landingTopHeight : topHeight;

		for (int h = 0; h < piece.size(); h++)
		{
			int currentRow = result.landingRow + h - result.linesCleared;
			content[currentRow] = piece[h] | content[currentRow];

			if (content[currentRow] == getCompleteLine())
			{
				result.pieceVanishedBlocks += activeBitsCount(piece[h]);
				removeRow(currentRow);
				result.linesCleared++;
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

		// THEN: we check if it's a game over situation, if it is we return -1, else we return the lowest row were the piece would fit
		return (landingRow + piece.size() > getHeight()) ? -1 : landingRow;
	}

	const std::vector<unsigned int>& Grid::getContent() const
	{
		return content;
	}

	unsigned int Grid::getCompleteLine() const
	{
		return (1 << getWidth()) - 1;
	}

	void Grid::removeRow(unsigned int index)
	{
		for (; index < getHeight() - 1; index++)
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
		output += activeBitsCount(content[getTopHeight() - 1] ^ 0); // Upper boundary of the grid
		output += activeBitsCount(getCompleteLine() ^ content[0]); // Lower boundary of the grid

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
			if (!(content[row] & highestOrderBit)) { output++; } // Left boundary of the grid (add 1 if the most left bit is 0)
			if (!(content[row] & 1)) { output++; } // Right boundary of the grid (add 1 if the most right bit is 0)

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
		int output(0), wellMask(3), wellPattern(2), gridWidth(getWidth());

		for (int col = 0; col < gridWidth; col++)
		{
			for (int row = getTopHeight() - 1; row >= 0; row--)
			{
				if ((content[row] & wellMask) == wellPattern)
				{
					output += 1 + emptyBlocksDown(row - 1, col);
				}
			}

			if (col == 0)
			{
				wellMask = 7;
				wellPattern = 5;
			}
			else if (col == gridWidth - 2)
			{
				wellMask = (1 << (gridWidth - 1)) + (1 << (gridWidth - 2)); // e.g 512 + 256 if W=10
				wellPattern = 1 << (gridWidth - 2); // e.g 256 if W=10
			}
			else
			{
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