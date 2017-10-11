#include "Heuristic.h"

namespace TetrisAI {

	const float Heuristic::gameOverEvaluation = -100000;

	float Heuristic::evaluate(const GameState& gs, float stopValue)
	{
		return evaluate(gs, stopValue, true);
	}

	float Heuristic::evaluate(const GameState& gs)
	{
		return evaluate(gs, 0, false);
	}

}