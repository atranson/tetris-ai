#ifndef TETRISAI_GAMESTATENODE_H
#define TETRISAI_GAMESTATENODE_H

#include "DecisionTreeNode.h"
#include "GameState.h"
#include <memory>
#include <vector>
#include <iostream>

namespace TetrisAI {

	class GameStateNode : public DecisionTreeNode {

	public:
		GameStateNode(GameState& gameState, int depth, std::vector<Polyomino>& possiblePolyominos) : gameState(gameState) { buildChildren(depth, possiblePolyominos); }
		virtual void updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos);
		virtual void movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination);
		virtual bool matchPolyomino(Polyomino* polyomino);
		virtual NodeStatus getNodeStatus();

		float getLocalEvaluation() const { return localEvaluation; };

	private:
		float localEvaluation;
		GameState gameState;
		std::vector<std::unique_ptr<DecisionTreeNode>> children;

		void buildChildren(int depth, std::vector<Polyomino>& possiblePolyominos);
	};

}

#endif
