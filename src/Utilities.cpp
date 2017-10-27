#include "Utilities.h"
#include <stdexcept>

namespace TetrisAI {

	int activeBitsCount(unsigned int value)
	{
		// Brian Kernighan's way as presentend here: http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
		unsigned int count;
		for (count = 0; value; count++)
		{
			value &= value - 1; // clear the least significant bit set
		}
		return count;
	}

	std::vector<int> splitRange(int start, unsigned length, unsigned parts)
	{
		unsigned q(length / parts), r(length % parts);
		if (length < parts)
		{
			parts = length;
		}

		std::vector<int> splitRangesBounds;
		splitRangesBounds.reserve(parts + 1);

		unsigned extras(r); // Allows to distribute the remainder r among the different parts one by one
		for (unsigned i = 0; i < parts; i++)
		{
			splitRangesBounds.push_back(start + i*q + r-extras);
			if (extras > 0) 
			{
				extras--; // One of the remainder has been distributed
			}
		}
		splitRangesBounds.push_back(start + length);

		return splitRangesBounds;
	}

}