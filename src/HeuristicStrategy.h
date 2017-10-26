#ifndef TETRISAI_HEURISTICSTRATEGY_H
#define TETRISAI_HEURISTICSTRATEGY_H

#include "AIStrategy.h"
#include "Heuristic.h"
#include "DecisionTreeNode.h"

namespace TetrisAI {

	class HeuristicStrategy : public AIStrategy {

	public:
		const static int maxDepth = 4;

		/// <param name="heuristic">Heuristic that should be used to eveluate decision tree nodes</param>
		/// <param name="depth">Number of moves to be considered in advance</param>
		/// <param name="useMultithreading">Toggle the use of multithreading to enhance the speed of decision making</param>
		HeuristicStrategy(Heuristic& heuristic, unsigned int depth, bool useMultithreading);

		/// <summary>Evaluate the possible outcomes from the given game state and outputs the best moves based on an heuristic</summary>
		/// <param name="gs">Game state which should have at least one pending polyomino to be played</param>
		/// <param name="possiblePolyominos">List of potential polyominos to populate the decision tree if needed</param>
		virtual Transformation decideMove(const GameState& gs, std::vector<Polyomino>& possiblePolyominos);

	private:
		/// <summary>Initialize the decision tree (should be called once before the first decision)</summary>
		/// <param name="gs">Game state that will serve as a basis for the decision tree</param>
		/// <param name="possiblePolyominos">List of potential polyominos to populate the decision tree if needed</param>
		void initializeTree(const GameState& gs, std::vector<Polyomino>& possiblePolyominos);

		/// <summary>Heuristic that should be used to evaluate game states</summary>
		Heuristic& heuristic;
		/// <summary>Number of moves to be considered in advance</summary>
		unsigned int depth;

		bool useMultithreading;
		std::unique_ptr<DecisionTreeNode> decisionTreeRoot;
	};

}

#endif
