#ifndef TETRISAI_MOVERESULT_H
#define TETRISAI_MOVERESULT_H

struct MoveResult {
	bool gameOver;
	short linesCleared;
	short pieceVanishedBlocks;
	short landingRow;

	MoveResult() : gameOver(false), linesCleared(0), pieceVanishedBlocks(0), landingRow(-1) {}
};

#endif
