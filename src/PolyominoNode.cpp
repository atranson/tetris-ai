#include "PolyominoNode.h"

namespace TetrisAI {

	PolyominoNode::PolyominoNode(GameState& gameState, Polyomino* p, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic) : polyomino(p)
	{
		GameState subGameState = gameState; // copy
		subGameState.addPolyominoToQueue(p);
		subRoot = std::make_unique<GameStateNode>(subGameState, depth, possiblePolyominos, heuristic);
	}

	void PolyominoNode::updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic, bool useMultithreading)
	{
		if (newPolyomino != nullptr)
		{
			throw std::invalid_argument("Error: PolyominoNode::updateTree should not receive newPolyominos since they represent cases where the polyomino is unknown.");
		}

		subRoot->updateTree(newPolyomino, depth, possiblePolyominos, heuristic, useMultithreading);
	}

	void PolyominoNode::movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination)
	{
		subRoot->movingChildrenOwnership(destination);
	}

	std::unique_ptr<DecisionTreeNode> PolyominoNode::extractBestChild()
	{
		return subRoot->extractBestChild();
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

	float PolyominoNode::getNodeEvaluation() const 
	{ 
		return subRoot->getNodeEvaluation(); 
	}

	Transformation PolyominoNode::getPolyominoMove() const
	{
		return subRoot->getPolyominoMove();
	}

	bool PolyominoNode::isGameOver() const
	{
		return subRoot->isGameOver();
	}

	// Compute a mean in an iterative way
	// X(t) = (t-1)/t * X(t-1) + x(t)/t  = [(t-1) * X(t-1) + x(t)]/t
	float PolyominoNode::computeSiblingsEvaluation(float currentEvaluation, unsigned nodePosition)
	{
		// Implicit conversion of nodePosition to float ensures correct computation
		return (nodePosition * currentEvaluation + getNodeEvaluation()) / (nodePosition + 1);
	}
}