#ifndef TETRISAI_HEURISTIC_H
#define TETRISAI_HEURISTIC_H

#include "GameState.h"

namespace TetrisAI {

	class Heuristic {
	public:
		static const float gameOverEvaluation;

		/// <summary>Outputs a value that grades the given GameState</summary>
		/// <param name="gs">Game state that should be evaluated</param>
		/// <returns>Evaluation of the game state</returns>
		virtual float evaluate(const GameState& gs) = 0;

		/// <summary>Outputs a value that grades the given GameState while considering its children's evaluation</summary>
		/// <param name="gs">Game state that should be evaluated</param>
		/// <param name="childrenEvaluation">Evaluation of the children of the corresponding GameStateNode</param>
		/// <returns>Evaluation of the game state that includes its children's evaluation</returns>
		virtual float evaluateBranch(const GameState& gs, float childrenEvaluation) = 0;
	};

}

#endif