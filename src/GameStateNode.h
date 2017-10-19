#ifndef TETRISAI_GAMESTATENODE_H
#define TETRISAI_GAMESTATENODE_H

#include "DecisionTreeNode.h"
#include "GameState.h"
#include <memory>
#include <vector>

namespace TetrisAI {

	class GameStateNode : public DecisionTreeNode {

	public:
		GameStateNode(GameState& gameState, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic);
		virtual void updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic);
		virtual void movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination);
		virtual bool matchPolyomino(Polyomino* polyomino);
		virtual NodeStatus getNodeStatus();
		virtual float getNodeEvaluation() const { return nodeEvaluation; }
		virtual float computeParentEvaluation(float currentEvaluation, unsigned nodePosition);

	private:
		float nodeEvaluation;
		GameState gameState;
		std::vector<std::unique_ptr<DecisionTreeNode>> children;

		void buildChildren(int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic);
		void updateNodeEvaluation(Heuristic& heuristic);
	};

}

#endif
