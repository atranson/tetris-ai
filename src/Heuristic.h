#ifndef TETRISAI_HEURISTIC_H
#define TETRISAI_HEURISTIC_H

#include "GameState.h"

namespace TetrisAI {

	class Heuristic {
	public:
		static const float gameOverEvaluation;
		float evaluate(const GameState& gs, float stopValue);
		float evaluate(const GameState& gs);

	private:
		virtual float evaluate(const GameState& gs, float stopValue, bool useStopValue) = 0;
	};

}

#endif
