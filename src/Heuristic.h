#ifndef TETRISAI_HEURISTIC_H
#define TETRISAI_HEURISTIC_H

#include "GameState.h"

namespace TetrisAI {

	class Heuristic {
	public:
		static const float gameOverEvaluation;

		/// <summary>Outputs a value that grades the given GameState</summary>
		/// <param name="gs">Game state that should be evaluated</param>
		/// <param name="stopValue">Threshold value that can be used to stop the computation if the output value falls below</param>
		/// <param name="useStopValue">Indicates if the computation should be stopped based on the stopValue</param>
		/// <returns>Evaluation of the game state</returns>
		virtual float evaluate(const GameState& gs) = 0;

		virtual float evaluateBranch(const GameState& gs, float childrenEvaluation) = 0;
	};

}

#endif