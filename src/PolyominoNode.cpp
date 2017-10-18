#include "PolyominoNode.h"

namespace TetrisAI {

	PolyominoNode::PolyominoNode(GameState& gameState, Polyomino* p, int depth, std::vector<Polyomino>& possiblePolyominosh) : polyomino(p)
	{
		GameState subGameState = gameState; // copy
		subGameState.addPolyominoToQueue(p);
		subRoot = std::make_unique<GameStateNode>(subGameState, depth, possiblePolyominosh);
	}

	void PolyominoNode::updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos)
	{
		if (newPolyomino != nullptr)
		{
			throw std::invalid_argument("Error: PolyominoNode::updateTree should not receive newPolyominos since they represent cases where the polyomino is unknown.");
		}

		subRoot->updateTree(newPolyomino, depth, possiblePolyominos);
	}

	void PolyominoNode::movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination)
	{
		subRoot->movingChildrenOwnership(destination);
	}

	bool PolyominoNode::matchPolyomino(Polyomino* p)
	{
		return p == polyomino;
	}

	DecisionTreeNode::NodeStatus PolyominoNode::getNodeStatus()
	{
		NodeCount firstLevel;
		firstLevel["PolyominoNode"] = 1;
		NodeStatus subRootStatus = subRoot->getNodeStatus();
		
		// We swap the count for the first level with the Count we created
		subRootStatus[0] = firstLevel;
		return subRootStatus;
	}
}