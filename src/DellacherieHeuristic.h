#ifndef TETRISAI_DELLACHERIEHEURISTIC_H
#define TETRISAI_DELLACHERIEHEURISTIC_H

#include "Heuristic.h"

namespace TetrisAI {

	class DellacherieHeuristic : public Heuristic {
	private:
		virtual float evaluate(const GameState& gs, float stopValue, bool useStopValue);
	};

}

#endif
