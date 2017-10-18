#include "GameState.h"
#include <algorithm>

namespace TetrisAI {


	GameState::GameState(short width, short height) : grid(width, height) {}
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

	const Polyomino& GameState::getPlayedPolyomino() const
	{
		return *playedPolyomino;
	}

	const Transformation& GameState::getPolyominoMove() const
	{
		return polyominoMove;
	}

	bool GameState::play(const Polyomino& polyomino, Transformation transformation)
	{
		// Reset state values
		playedPolyomino = &polyomino;
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

	void GameState::addPolyominoToQueue(Polyomino* polyomino)
	{
		polyominoQueue.push_back(polyomino);
	}

	int GameState::getPolyominoQueueSize() const
	{
		return polyominoQueue.size();
	}

	Polyomino* GameState::popPolyominoQueue()
	{
		if (!polyominoQueue.empty())
		{
			Polyomino* head(polyominoQueue.front());
			polyominoQueue.pop_front();
			return head;
		}
		return nullptr;
	}
}