#include "PolyominoState.h"
#include <algorithm>
#include <stdexcept>

namespace TetrisAI {
	PolyominoState::PolyominoState(std::vector<unsigned int> content) : content(content) 
	{ 
		for (auto &row : content) // access by reference to avoid copying
		{
			if (!row)
			{
				throw std::invalid_argument("All rows of a polyomino state must be non empty");
			}
		}
		computeWidth(); 
	}
	
	std::vector<unsigned int> PolyominoState::getTranslatedState(int translation) const
	{
		std::vector<unsigned int> translatedState = content; // first copy the pattern
		for (auto& row : translatedState)
		{
			row <<= translation; // multiply each line by 2^translation
		}
		return translatedState;
	}

	int PolyominoState::getHeight() const
	{
		return content.size();
	}

	int PolyominoState::getWidth() const
	{
		return width;
	}

	PolyominoState PolyominoState::getRotatedState()
	{
		PolyominoState copy = *this;
		copy.transpose();
		copy.reverseLines();
		return copy;
	}

	void PolyominoState::computeWidth()
	{
		// To retrieve the width, we have to find the highest bit set to 1
		// Since in a Polyomino, blocks are adjacent, we just need to find the row with the highest value
		unsigned int maxRow = *std::max_element(content.begin(), content.end());

		// If the row is not null, the width is at least 1, we increment until we have an empty row
		width = maxRow ? 1 : 0;
		while (maxRow >>= 1)
		{
			width++;
		}
	}

	void PolyominoState::transpose()
	{
		std::vector<unsigned int> newContent(getWidth(), 0); // New height is equal to the current width
		for (int row = 0; row < getHeight(); row++)
		{
			for (int col = 0; col < getWidth(); col++)
			{
				if (content[row] & (1 << col))
				{
					newContent[col] += (1 << row);
				}
			}
		}

		// Before changing the content, we swap width and height
		width = getHeight();
		content = newContent;
	}

	// Inspired by https://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious
	void PolyominoState::reverseLines()
	{
		for (int row = 0; row < getHeight(); row++)
		{
			unsigned int baseValue = content[row], extraShift(getWidth() - 1);
			content[row] = baseValue;
			for (baseValue >>= 1; baseValue; baseValue >>= 1)
			{
				content[row] <<= 1;
				content[row] |= baseValue & 1;
				extraShift--;
			}
			content[row] <<= extraShift;
			content[row] &= (1 << getWidth()) - 1; // Keep only the first n-bits with n=width
		}
	}

	bool PolyominoState::isEqual(PolyominoState const & b) const
	{
		if (getHeight() == b.getHeight() && getWidth() == b.getWidth())
		{
			for (int row = 0; row < getHeight(); row++)
			{
				if (content[row] != b.content[row])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

}