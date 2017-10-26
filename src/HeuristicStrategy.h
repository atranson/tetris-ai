#ifndef TETRISAI_HEURISTICSTRATEGY_H
#define TETRISAI_HEURISTICSTRATEGY_H

#include "Polyomino.h"
#include "GameState.h"
#include "AIStrategy.h"
#include "Heuristic.h"
#include "DecisionTreeNode.h"

namespace TetrisAI {

	class HeuristicStrategy : public AIStrategy {

	public:
		const static int maxDepth = 4;

		HeuristicStrategy(Heuristic& heuristic, unsigned int depth, bool useMultithreading);
		virtual Transformation decideMove(const GameState& gs, std::vector<Polyomino>& possiblePolyominos);

	private:
		void initializeTree(const GameState& gs, std::vector<Polyomino>& possiblePolyominos);

		/// <summary>Heuristic that should be used to evaluate game states</summary>
		Heuristic& heuristic;
		unsigned int depth;
		bool useMultithreading;
		std::unique_ptr<DecisionTreeNode> decisionTreeRoot;
	};

}

#endif
