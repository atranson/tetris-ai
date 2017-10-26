#include "GridTools.h"

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

}