#ifndef TETRISAI_MOVERESULT_H
#define TETRISAI_MOVERESULT_H

struct MoveResult {
	bool gameOver;
	short linesCleared;
	short pieceVanishedBlocks;
	short landingRow;

	MoveResult() : gameOver(false), linesCleared(0), pieceVanishedBlocks(0), landingRow(-1) {}
	MoveResult(bool gameOver, short linesCleared, short pieceVanishedBlocks, short landingRow) : 
		gameOver(gameOver), linesCleared(linesCleared), pieceVanishedBlocks(pieceVanishedBlocks), landingRow(landingRow) {}

	bool isEqual(MoveResult const& b) const {
		return gameOver == b.gameOver && linesCleared == b.linesCleared && pieceVanishedBlocks == b.pieceVanishedBlocks && landingRow == b.landingRow;
	}
};

inline bool operator==(const MoveResult& lhs, const MoveResult& rhs) { return lhs.isEqual(rhs); }

#endif
