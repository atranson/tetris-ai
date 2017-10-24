#include "DellacherieHeuristic.h"

namespace TetrisAI {
	
	float DellacherieHeuristic::evaluate(const GameState& gs)
	{
		if (gs.isGameOver() || gs.getPlayedPolyomino() == nullptr)
		{
			return Heuristic::gameOverEvaluation;
		}

		const Grid& grid(gs.getGrid());
		MoveResult moveResult(gs.getMoveResult());
		float output(0), topMarginMax, topMarginMin;
		
		// Heuristic given by Pierre Dellacherie's algorithm
		topMarginMin = grid.getHeight() + 1 - moveResult.landingRow; 
		topMarginMax = grid.getHeight() + 1 - (moveResult.landingRow + gs.getPlayedPolyomino()->getRotatedPiece(gs.getPolyominoMove().rotation).getHeight() - 1);
		output += (topMarginMin + topMarginMin) / 2;
		output += 2 * moveResult.linesCleared * moveResult.pieceVanishedBlocks;
		
		output -= 2 * (grid.getHeight() + 1 - grid.getTopHeight());
		output -= grid.columnTransitions();
		output -= grid.rowTransitions();
		output -= 4 * grid.cellars();
		output -= grid.wells();
		
		return output;
	}

	float DellacherieHeuristic::evaluateBranch(const GameState& gs, float childrenEvaluation)
	{
		MoveResult moveResult(gs.getMoveResult());
		return childrenEvaluation + 2 * moveResult.linesCleared * moveResult.pieceVanishedBlocks;
	}
}