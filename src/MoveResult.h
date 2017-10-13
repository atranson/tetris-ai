#ifndef TETRISAI_MOVERESULT_H
#define TETRISAI_MOVERESULT_H

struct MoveResult {
	/// <summary>Indicates if the move resulted in a game over</summary>
	bool gameOver;
	/// <summary>Number of rows that were cleared consecutively to the move</summary>
	short linesCleared;
	/// <summary>Number of squares of the polyomino that was used for the move that disappeared in cleared rows</summary>
	short pieceVanishedBlocks;
	/// <summary>Index of the row where the polyomino landed</summary>
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
