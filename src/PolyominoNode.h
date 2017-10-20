#ifndef TETRISAI_POLYOMINONODE_H
#define TETRISAI_POLYOMINONODE_H

#include "DecisionTreeNode.h"
#include "GameStateNode.h"
#include "GameState.h"
#include "Polyomino.h"
#include <memory>

namespace TetrisAI {

	class PolyominoNode : public DecisionTreeNode {

	public:
		PolyominoNode(GameState& gameState, Polyomino* p, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic);
		virtual void updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic);
		virtual void movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination);
		virtual std::unique_ptr<DecisionTreeNode> extractBestChild();
		virtual bool matchPolyomino(Polyomino* polyomino);
		virtual NodeStatus getNodeStatus();
		virtual float getNodeEvaluation() const;
		virtual float computeParentEvaluation(float currentEvaluation, unsigned nodePosition);

	private:
		Polyomino* polyomino;
		std::unique_ptr<GameStateNode> subRoot;
	};

}

#endif
