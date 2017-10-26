#ifndef TETRISAI_GAMESEQUENCE_H
#define TETRISAI_GAMESEQUENCE_H

#include <mutex>
#include <atomic>
#include <memory>
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
		const static unsigned maxStepsAhead = 5;

		enum Status {
			New,
			Playing,
			GameOver
		};

		GameSequence(short gridWidth, short gridHeight, unsigned int polyominoSquares, std::shared_ptr<AIStrategy> strategy, unsigned int stepsAhead);

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
		/// <summary>Number of polyominos known in advance during the game (e.g. if set to 0, the next polyomino is unknown)</summary>
		unsigned int stepsAhead;
		short gridWidth;
		short gridHeight;
		std::shared_ptr<AIStrategy> strategy;

		/// <summary>Holds statistics about the game being played</summary>
		GameStatistics stats;
		/// <summary>Stores the current state of the game being played</summary>
		GameState gameState;
		/// <summary>Stores the current status of the game being played</summary>
		std::atomic<Status> status;

		void playMove(Transformation transformation);
		void updateStatistics(int playedPolyominoIndex, const MoveResult& result);
	};

}

#endif
