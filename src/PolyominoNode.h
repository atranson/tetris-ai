#ifndef TETRISAI_POLYOMINONODE_H
#define TETRISAI_POLYOMINONODE_H

#include "DecisionTreeNode.h"
#include "GameStateNode.h"
#include "GameState.h"
#include "Polyomino.h"
#include <memory>

namespace TetrisAI {

	/// <summary>
	/// This class of nodes is meant to handle moves for which the polyomino is yet unknown.
	/// It considers one of the potential polyomino that could be drawn and builds from there
	/// a sub tree of possibilities (thus, this can't be a leaf nor a root)
	///
	/// It is meant to be destroyed or hand over the children of its subRoot once the polyomino corresponding
	/// to that move is eventually known.
	/// </summary>
	class PolyominoNode : public DecisionTreeNode {

	public:
		PolyominoNode(GameState& gameState, Polyomino* p, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic);
		virtual void updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic, bool useMultithreading);
		virtual void movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination);
		virtual std::unique_ptr<DecisionTreeNode> extractBestChild();
		virtual bool matchPolyomino(Polyomino* polyomino);
		virtual NodeStatus getNodeStatus();
		virtual float getNodeEvaluation() const;
		virtual Transformation getPolyominoMove() const;
		virtual bool isGameOver() const;
		virtual float computeSiblingsEvaluation(float currentEvaluation, unsigned nodePosition);

	private:
		/// <summary>Polyomino that is considered for this node and its children</summary>
		Polyomino* polyomino;
		/// <summary>Sub decision tree based on the case where the next polyomino is the one referenced in this instance</summary>
		std::unique_ptr<GameStateNode> subRoot;
	};

}

#endif
