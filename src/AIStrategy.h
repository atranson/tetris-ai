#ifndef TETRISAI_AISTRATEGY_H
#define TETRISAI_AISTRATEGY_H

#include "Polyomino.h"
#include "GameState.h"

namespace TetrisAI {

	class AIStrategy {
	public:
		virtual Transformation decideMove(const GameState& gs, const Polyomino& nextPolyomino) = 0;
	};

}

#endif
