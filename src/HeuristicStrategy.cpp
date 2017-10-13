#include "HeuristicStrategy.h"

namespace TetrisAI {

	Transformation HeuristicStrategy::decideMove(const GameState& gs, const Polyomino& nextPolyomino)
	{
		float currentBest(heuristic.gameOverEvaluation);
		Transformation t, bestMove(-1, -1);

		// Test each possible move for the given polyomino and keep the one that has the best evaluation
		for (t.rotation = 0; t.rotation < nextPolyomino.getRotationCount(); t.rotation++)
		{
			PolyominoState rotatedPolyomino = nextPolyomino.getRotatedPiece(t.rotation);
			for (t.translation = 0; t.translation <= gs.getGrid().getWidth() - rotatedPolyomino.getWidth(); t.translation++)
			{
				// Create a new game state where that move was played
				GameState postMoveState = gs;
				postMoveState.play(nextPolyomino, t);

				// Evaluate the new game state and compare its evaluation to the best one so far
				float evaluation(heuristic.evaluate(postMoveState, currentBest));
				if (currentBest < evaluation)
				{
					currentBest = evaluation;
					bestMove = t;
				}
			}
		}

		return bestMove;
	}

}