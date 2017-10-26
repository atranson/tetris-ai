#ifndef TETRISAI_AISTRATEGY_H
#define TETRISAI_AISTRATEGY_H

#include "Polyomino.h"
#include "GameState.h"
#include <vector>

namespace TetrisAI {

	class AIStrategy {
	public:
		/// <summary>Decides which move is the best and returns the corresponding transformation</summary>
		/// <param name="gs">GameState in which the polyomino must be played</param>
		/// <returns>The polyomino's transformation that corresponds to the best move according to the AI</returns>
		virtual Transformation decideMove(const GameState& gs, std::vector<Polyomino>& possiblePolyominos) = 0;
	};

}

#endif
