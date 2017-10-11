#ifndef TETRISAI_GAMESTATE_H
#define TETRISAI_GAMESTATE_H

#include <memory>
#include <vector>
#include "Polyomino.h"
#include "MoveResult.h"
#include "Grid.h"
#include "GridTools.h"

namespace TetrisAI {

	class GameState {

	private:
		const Polyomino* currentPolyomino;
		Transformation polyominoMove;
		MoveResult moveResult;
		Grid grid;

	public:
		GameState(short width, short height);
		GameState(GameState const & original);

		const std::vector<unsigned int>& getGridContent() const;
		const Grid& getGrid() const;
		const MoveResult& getMoveResult() const;
		const Polyomino& getCurrentPolyomino() const;
		const Transformation& getPolyominoMove() const;
		bool play(const Polyomino& polyomino, Transformation transformation);
		bool isGameOver() const;
	};

}

#endif
