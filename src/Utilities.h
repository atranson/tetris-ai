#ifndef TETRISAI_UTILITIES_H
#define TETRISAI_UTILITIES_H

#include <vector>

namespace TetrisAI {

	/// <summary>Returns the count of bits set to 1 in the given value</summary>
	int activeBitsCount(unsigned int value);

	/// <summary>Divide the given range into a number of smaller ranges and outputs a vector storing the starting point of those subranges</summary>
	/// <param name="start">Start of the range</param>
	/// <param name="length">Length of the range</param>
	/// <param name="parts">Number of parts in which the range should be divided</param>
	/// <returns>A vector containing the start of every sub ranges + the end point (start + length)</returns>
	/// <remarks>
	/// The given output is built so that the couple (output[i], output[i+1]-1) represents the bounds of the sub range i
	/// Thus, this output is meant to be browsed from index 0 to size - 1
	/// Moreover, the number of sub ranges given might be lower than parts if length given is lower than the number of asked parts
	/// </remarks>
	std::vector<int> splitRange(int start, unsigned length, unsigned parts);
}

#endif
