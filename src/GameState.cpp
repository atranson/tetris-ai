#include "GameState.h"
#include <algorithm>

namespace TetrisAI {


	GameState::GameState(short width, short height) : grid(width, height) {}
	GameState::GameState(const GameState &original) : grid(original.grid), moveResult(original.moveResult) {}

	const std::vector<unsigned int>& GameState::getGridContent() const
	{
		return grid.getContent();
	}

	const Grid& GameState::getGrid() const
	{
		return grid;
	}

	const MoveResult& GameState::getMoveResult() const
	{
		return moveResult;
	}

	const Polyomino& GameState::getCurrentPolyomino() const
	{
		return *currentPolyomino;
	}

	const Transformation& GameState::getPolyominoMove() const
	{
		return polyominoMove;
	}

	bool GameState::play(const Polyomino& polyomino, Transformation transformation)
	{
		// Reset state values
		currentPolyomino = &polyomino;
		polyominoMove = transformation;
		PolyominoState polyominoState(polyomino.getRotatedPiece(transformation.rotation));

		moveResult = grid.fitPiece(polyomino, transformation);

		// Return true if the piece could fit
		return !moveResult.gameOver;
	}

	bool GameState::isGameOver() const
	{
		return moveResult.gameOver;
	}
}