#ifndef TETRISAI_HEURISTICSTRATEGY_H
#define TETRISAI_HEURISTICSTRATEGY_H

#include "Polyomino.h"
#include "GameState.h"
#include "AIStrategy.h"
#include "Heuristic.h"

namespace TetrisAI {

	class HeuristicStrategy : public AIStrategy {

	public:
		HeuristicStrategy(Heuristic& heuristic) : heuristic(heuristic) {}
		virtual Transformation decideMove(const GameState& gs, const Polyomino& nextPolyomino);

	private:
		Heuristic& heuristic;
	};

}

#endif
