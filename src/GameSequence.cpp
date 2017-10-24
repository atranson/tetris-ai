#include "GameSequence.h"
#include <time.h> 
#include <iostream>

namespace TetrisAI {

	const GameStatistics GameSequence::getStats() const
	{
		std::lock_guard<std::mutex> guard(mutex);
		return stats;
	}

	const GameState GameSequence::getGameState() const
	{
		std::lock_guard<std::mutex> guard(mutex);
		return gameState;
	}

	const GameSequence::Status GameSequence::getStatus() const
	{
		return status;
	}

	void GameSequence::playMove(Transformation transformation)
	{
		std::lock_guard<std::mutex> guard(mutex);
		gameState.play(transformation);
	}

	int GameSequence::getGridWidth() const
	{
		return gridWidth;
	}

	int GameSequence::getGridHeight() const
	{
		return gridHeight;
	}

	void GameSequence::updateStatistics(int playedPolyominoIndex, const MoveResult& result)
	{
		std::lock_guard<std::mutex> guard(mutex);
		stats.polyominosPlayed++;
		stats.polyominosBreakdown[playedPolyominoIndex]++;
		if (result.linesCleared != 0)
		{
			stats.linesCleared += result.linesCleared;
			stats.linesClearedBreakdown[result.linesCleared - 1]++;
		}
	}

	void GameSequence::playGame()
	{
		srand(time(NULL));
		status = GameSequence::Status::Playing;
		std::vector<Polyomino> polyominos(Polyomino::getPolyominosList(polyominoSquares));
		stats.polyominosBreakdown = std::vector<unsigned int>(polyominos.size());

		// We know two pieces in advance, thus we add one here
		int currentPolyominoIndex(0);
		
		// Draw in advance a certain amount of polyominos
		for (int i = 0; i < stepsAhead; i++)
		{
			currentPolyominoIndex = rand() % polyominos.size();
			gameState.addPolyominoToQueue(&(polyominos[currentPolyominoIndex]));
		}

		while (status == GameSequence::Status::Playing)
		{
			currentPolyominoIndex = rand() % polyominos.size();
			gameState.addPolyominoToQueue(&(polyominos[currentPolyominoIndex]));
			Transformation chosenMove(strategy->decideMove(gameState, polyominos));

			// If a valid move was found
			if (chosenMove.translation != -1)
			{
				//std::cout << "Chose (" << chosenMove.translation << ", " << chosenMove.rotation << ") for Piece #" << currentPolyominoIndex << std::endl;
				playMove(chosenMove);
				
				// Updating statistics
				updateStatistics(currentPolyominoIndex, gameState.getMoveResult());
				if (stats.polyominosPlayed % 100000 == 0) 
				{
					std::cout << "Played " << stats.polyominosPlayed << " - Cleared " << stats.linesCleared << std::endl;
				}
			}
			else
			{
				//std::cout << "Game over: could not place Piece #" << currentPolyominoIndex << std::endl;
				status = GameSequence::Status::GameOver;
			}
		}
	}
}