#ifndef TETRISAI_GAMESTATE_H
#define TETRISAI_GAMESTATE_H

#include <vector>
#include <deque>
#include "Polyomino.h"
#include "MoveResult.h"
#include "Grid.h"
#include "GridTools.h"

namespace TetrisAI {

	class GameState {

	private:
		/// <summary>Polyomino that was played to produce that state</summary>
		const Polyomino* playedPolyomino;
		/// <summary>Transformation that was applied to the played polyomino</summary>
		Transformation polyominoMove;
		/// <summary>Result obtained from the landing of the Polyomino in the grid</summary>
		MoveResult moveResult;
		/// <summary>Grid after the landing of the Polyomino</summary>
		Grid grid;
		/// <summary>Queue of known polyominos</summary>
		std::deque<Polyomino*> polyominoQueue;

	public:
		GameState(short width, short height);
		GameState(GameState const & original);

		const std::vector<unsigned int>& getGridContent() const;
		const Grid& getGrid() const;
		const MoveResult& getMoveResult() const;
		const Polyomino& getPlayedPolyomino() const;
		const Transformation& getPolyominoMove() const;
		bool isGameOver() const;

		void addPolyominoToQueue(Polyomino* polyomino);
		int getPolyominoQueueSize() const;
		Polyomino* popPolyominoQueue();

		/// <summary>Fit the transformed polyomino in the grid and update all status accordingly</summary>
		/// <param name="polyomino">Polyomino that must land in the grid</param>
		/// <param name="transformation">Transformation to be applied before playing the Polyomino</param>
		/// <returns>True if the move could be played, False if it resulted in a game over</returns>
		bool play(const Polyomino& polyomino, Transformation transformation);
	};

}

#endif
