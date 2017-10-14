#ifndef TETRISAI_GAMESEQUENCE_H
#define TETRISAI_GAMESEQUENCE_H

#include <mutex>
#include <atomic>
#include "AIStrategy.h"
#include "GameState.h"

namespace TetrisAI {

	struct GameStatistics {
		/// <summary>Number of polyominos that were played during the game</summary>
		unsigned int polyominosPlayed;
		
		/// <summary>Number of lines that were cleared during the game</summary>
		unsigned int linesCleared;

		/// <summary>List of n elements if polyominos are composed of n squares. Entry i contains the number of moves that cleared i rows at once</summary>
		std::vector<unsigned int> linesClearedBreakdown;

		/// <summary>Entry i contains the number of times the polyomino indexed at i has been played</summary>
		std::vector<unsigned int> polyominosBreakdown;

		GameStatistics(unsigned int polyominosSquares) : polyominosPlayed(0), linesCleared(0), linesClearedBreakdown(polyominosSquares) {}
	};

	class GameSequence {

	public:
		enum Status {
			New,
			Playing,
			GameOver
		};

		GameSequence(short gridWidth, short gridHeight, unsigned int polyominoSquares, AIStrategy* strategy, bool waitBetweenMoves=false) :
			gridWidth(gridWidth), gridHeight(gridHeight), polyominoSquares(polyominoSquares), 
			strategy(strategy), stats(polyominoSquares), status(Status::New),
			gameState(gridWidth, gridHeight), waitBetweenMoves(waitBetweenMoves)
			{}

		// Those getters return values since we may have concurrency problem
		const GameStatistics getStats() const;
		const GameState getGameState() const;
		const Status getStatus() const;

		int getGridWidth() const;
		int getGridHeight() const;

		/// <summary>Plays a game of Tetris with the given AI until a game over is encountered</summary>
		void playGame();

	private:
		mutable std::mutex mutex;

		/// <summary>Number of squares that should compose the polyominos used for the game</summary>
		unsigned int polyominoSquares;
		short gridWidth;
		short gridHeight;
		AIStrategy* strategy;
		GameStatistics stats;
		GameState gameState;
		bool waitBetweenMoves;
		std::atomic<Status> status;

		void playMove(const Polyomino& polyomino, Transformation transformation);
		void updateStatistics(int playedPolyominoIndex, const MoveResult& result);
	};

}

#endif
