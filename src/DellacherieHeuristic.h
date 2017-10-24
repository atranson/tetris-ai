#ifndef TETRISAI_DELLACHERIEHEURISTIC_H
#define TETRISAI_DELLACHERIEHEURISTIC_H

#include "Heuristic.h"

namespace TetrisAI {

	class DellacherieHeuristic : public Heuristic {
	public:
		virtual float evaluate(const GameState& gs);
		virtual float evaluateBranch(const GameState& gs, float childrenEvaluation);
	};

}

#endif
