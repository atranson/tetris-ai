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

	void GameSequence::playMove(const Polyomino& polyomino, Transformation transformation)
	{
		std::lock_guard<std::mutex> guard(mutex);
		gameState.play(polyomino, transformation);
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

		while (status == GameSequence::Status::Playing)
		{
			int currentPolyominoIndex = rand() % polyominos.size();
			Polyomino& currentPolyomino(polyominos[currentPolyominoIndex]);
			Transformation chosenMove(strategy->decideMove(gameState, currentPolyomino));

			// If a valid move was found
			if (chosenMove.translation != -1)
			{
				//std::cout << "Chose (" << chosenMove.translation << ", " << chosenMove.rotation << ") for Piece #" << currentPolyominoIndex << std::endl;
				playMove(currentPolyomino, chosenMove);
				
				// Updating statistics
				updateStatistics(currentPolyominoIndex, gameState.getMoveResult());
				if (stats.polyominosPlayed % 10000 == 0) 
				{
					std::cout << "Played " << stats.polyominosPlayed << " - Cleared " << stats.linesCleared << std::endl;
				}
			}
			else
			{
				//std::cout << "Game over: could not place Piece #" << currentPolyominoIndex << std::endl;
				status = GameSequence::Status::GameOver;
			}

			if(waitBetweenMoves)
			{ 
				system("pause");
			}
		}
	}
}