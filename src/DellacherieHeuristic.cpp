#include "DellacherieHeuristic.h"

namespace TetrisAI {
	
	float DellacherieHeuristic::evaluate(const GameState& gs, float stopValue, bool useStopValue)
	{
		if (gs.isGameOver())
		{
			return Heuristic::gameOverEvaluation;
		}

		Grid grid(gs.getGrid());
		MoveResult moveResult(gs.getMoveResult());
		float output(0), topMarginMax, topMarginMin;

		// Heuristic given by Pierre Dellacherie's algorithm
		topMarginMin = grid.getHeight() + 1 - moveResult.landingRow;
		topMarginMax = grid.getHeight() + 1 - (moveResult.landingRow + gs.getCurrentPolyomino().getRotatedPiece(gs.getPolyominoMove().rotation).getHeight() - 1);
		output += (topMarginMin + topMarginMin) / 2;
		output += 2 * moveResult.linesCleared * moveResult.pieceVanishedBlocks;

		if (!useStopValue || stopValue < output) { output -= 2 * (grid.getHeight() + 1 - grid.getTopHeight()); }
		if (!useStopValue || stopValue < output) { output -= grid.columnTransitions(); }
		if (!useStopValue || stopValue < output) { output -= grid.rowTransitions(); }
		if (!useStopValue || stopValue < output) { output -= 4 * grid.cellars(); }
		if (!useStopValue || stopValue < output) { output -= grid.wells(); }

		return output;
	}
}