#include "GameState.h"

namespace TetrisAI {


	GameState::GameState(short width, short height) : grid(width, height), playedPolyomino(nullptr) {}
	GameState::GameState(const GameState &original) : 
		grid(original.grid), 
		moveResult(original.moveResult),
		playedPolyomino(original.playedPolyomino),
		polyominoMove(original.polyominoMove),
		polyominoQueue(original.polyominoQueue)
	{}

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

	const Polyomino* GameState::getPlayedPolyomino() const
	{
		return playedPolyomino;
	}

	const Transformation& GameState::getPolyominoMove() const
	{
		return polyominoMove;
	}

	bool GameState::play(Transformation transformation)
	{
		// Reset state values
		playedPolyomino = polyominoQueueHead();
		polyominoQueue.pop_front(); // Delete it from the queue as we will use it here
		polyominoMove = transformation;
		moveResult = grid.fitPiece(*playedPolyomino, transformation);

		// Return true if the piece could fit
		return !moveResult.gameOver;
	}

	bool GameState::isGameOver() const
	{
		return moveResult.gameOver;
	}

	void GameState::addPolyominoToQueue(Polyomino* polyomino)
	{
		polyominoQueue.push_back(polyomino);
	}

	int GameState::getPolyominoQueueSize() const
	{
		return polyominoQueue.size();
	}

	Polyomino* GameState::polyominoQueueHead() const
	{
		if (!polyominoQueue.empty())
		{
			return polyominoQueue.front();
		}
		return nullptr;
	}

	Polyomino* GameState::polyominoQueueTail() const
	{
		if (!polyominoQueue.empty())
		{
			return polyominoQueue.back();
		}
		return nullptr;
	}
}