#ifndef TETRISAI_POLYOMINOSTATE_H
#define TETRISAI_POLYOMINOSTATE_H

#include <vector>

namespace TetrisAI {

	class PolyominoState {

	public:
		PolyominoState(std::vector<unsigned int> content);
		PolyominoState(const PolyominoState & original) : content(original.content), width(original.width) {}
		std::vector<unsigned int> getTranslatedState(int translation) const;
		int getHeight() const;
		int getWidth() const;
		// Clockwise 90 degrees rotation
		PolyominoState getRotatedState();

		bool isEqual(PolyominoState const& b) const;

	private:
		std::vector<unsigned int> content;
		short width;

		void computeWidth();
		void transpose();
		void reverseLines();
	};

	inline bool operator==(const PolyominoState& lhs, const PolyominoState& rhs) { return lhs.isEqual(rhs); }

}

#endif
