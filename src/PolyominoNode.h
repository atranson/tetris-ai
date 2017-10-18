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
		PolyominoNode(GameState& gameState, Polyomino* p, int depth, std::vector<Polyomino>& possiblePolyominos);
		virtual void updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos);
		virtual void movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination);
		virtual bool matchPolyomino(Polyomino* polyomino);
		virtual NodeStatus getNodeStatus();

	private:
		Polyomino* polyomino;
		std::unique_ptr<GameStateNode> subRoot;
	};

}

#endif