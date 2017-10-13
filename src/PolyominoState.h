#ifndef TETRISAI_POLYOMINOSTATE_H
#define TETRISAI_POLYOMINOSTATE_H

#include <vector>

namespace TetrisAI {

	class PolyominoState {

	public:
		PolyominoState(std::vector<unsigned int> content);
		PolyominoState(const PolyominoState & original) : content(original.content), width(original.width) {}
		
		/// <summary>Translates a copy of the underlying raw content and returns it</summary>
		std::vector<unsigned int> getTranslatedState(int translation) const;
		
		/// <summary>Performs a lockwise 90 degrees rotation of the content and returns a new PolyominoState with the resulting content</summary>
		PolyominoState getRotatedState();

		int getHeight() const;
		int getWidth() const;
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
