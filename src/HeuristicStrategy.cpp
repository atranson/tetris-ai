#include "HeuristicStrategy.h"

namespace TetrisAI {

	Transformation HeuristicStrategy::decideMove(const GameState& gs, const Polyomino& nextPolyomino)
	{
		float currentBest(heuristic.gameOverEvaluation);
		Transformation t, bestMove(-1, -1);
		for (t.rotation = 0; t.rotation < nextPolyomino.getRotationCount(); t.rotation++)
		{
			PolyominoState rotatedPolyomino = nextPolyomino.getRotatedPiece(t.rotation);
			for (t.translation = 0; t.translation <= gs.getGrid().getWidth() - rotatedPolyomino.getWidth(); t.translation++)
			{
				GameState postMoveState = gs;
				postMoveState.play(nextPolyomino, t);

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