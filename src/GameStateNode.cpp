#include "GameStateNode.h"
#include "PolyominoNode.h"
#include <stdexcept>

namespace TetrisAI {

	GameStateNode::GameStateNode(GameState& gameState, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic) : gameState(gameState) 
	{ 
		buildChildren(depth, possiblePolyominos, heuristic); 
		updateNodeEvaluation(heuristic); 
	}

	void GameStateNode::buildChildren(int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic)
	{
		// If it a game over, we can't build anything from there
		// If depth is 0, we don't have anything to build, just to evaluate the current state
		if (depth == 0 || gameState.isGameOver())
		{
			return;
		}

		// If the list of children is not empty, we clear it beforehand
		if (!children.empty())
		{
			children.clear();
		}

		GameState newBaseGameState = gameState; // Copy game state
		Polyomino* comingPolyomino(newBaseGameState.popPolyominoQueue()); // Modify the queue and retrieve upcoming polyomino
		// If the queue was empty
		if (comingPolyomino == nullptr)
		{
			for (auto& p : possiblePolyominos)
			{
				children.push_back(std::make_unique<PolyominoNode>(newBaseGameState, &p, depth, possiblePolyominos, heuristic));
			}
		}
		else
		{
			Transformation t;

			// Test each possible move for the given polyomino and keep the one that has the best evaluation
			for (t.rotation = 0; t.rotation < comingPolyomino->getRotationCount(); t.rotation++)
			{
				PolyominoState rotatedPolyomino = comingPolyomino->getRotatedPiece(t.rotation);
				for (t.translation = 0; t.translation <= newBaseGameState.getGrid().getWidth() - rotatedPolyomino.getWidth(); t.translation++)
				{
					// Create a new game state where that move was played
					GameState postMoveState = newBaseGameState;
					postMoveState.play(*comingPolyomino, t);

					children.push_back(std::make_unique<GameStateNode>(postMoveState, depth - 1, possiblePolyominos, heuristic));
				}
			}
		}
	}

	void GameStateNode::updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic)
	{
		if (depth == 0)
		{
			return;
		}

		int queueSizeBeforeUpdate = gameState.getPolyominoQueueSize();
		if (newPolyomino != nullptr)
		{
			gameState.addPolyominoToQueue(newPolyomino);
		}

		if (children.empty())
		{
			buildChildren(depth, possiblePolyominos, heuristic);
		}
		else
		{
			if (queueSizeBeforeUpdate == 0 && newPolyomino != nullptr)
			{
				bool found = false;
				std::unique_ptr<DecisionTreeNode> matchingChild(nullptr);
				while(!children.empty() && matchingChild == nullptr)
				{
					if (children.back()->matchPolyomino(newPolyomino))
					{
						matchingChild = std::move(children.back());
					}
					children.pop_back();
				}

				if (matchingChild == nullptr)
				{
					throw std::runtime_error("Error:  could not find a match for a certain polyomino in the tree decision. Tree state unexpected. This layer should be composed of PolyominoNodes with one for each possible polyomino.");
				}

				children.clear();
				matchingChild->movingChildrenOwnership(children);
				newPolyomino = nullptr; // Polyomino has been "used" at this level of depth
			}

			for (auto& child : children)
			{
				child->updateTree(newPolyomino, depth - 1, possiblePolyominos, heuristic);
			}
		}

		updateNodeEvaluation(heuristic);
	}

	void GameStateNode::updateNodeEvaluation(Heuristic& heuristic)
	{
		if (children.empty())
		{
			nodeEvaluation = heuristic.evaluate(gameState);
		}
		else
		{
			// Actualize the evaluation of the node based on its children
			nodeEvaluation = 0; // Initialize the evaluation of the node
			int childNodePosition(0);
			for (auto& child : children)
			{
				nodeEvaluation = child->computeParentEvaluation(nodeEvaluation, childNodePosition);
				childNodePosition++;
			}
		}
	}

	void GameStateNode::movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination)
	{
		while (!children.empty())
		{
			destination.push_back(std::move(children.back()));
			children.pop_back();
		}
	}

	std::unique_ptr<DecisionTreeNode> GameStateNode::extractBestChild()
	{
		if (children.empty())
		{
			return std::unique_ptr<DecisionTreeNode>(nullptr);
		}

		float best(children[0]->getNodeEvaluation());
		int bestIndex(0);
		for (int i=1; i<children.size(); i++)
		{
			float childEvaluation(children[i]->getNodeEvaluation());
			if (childEvaluation > best)
			{
				bestIndex = i;
				best = childEvaluation;
			}
		}

		return std::move(children[bestIndex]);
	}

	bool GameStateNode::matchPolyomino(Polyomino* polyomino)
	{
		return false;
	}

	DecisionTreeNode::NodeStatus GameStateNode::getNodeStatus()
	{
		NodeStatus output;
		output.push_back(NodeCount());
		output[0]["GameStateNode"] = 1;
		NodeStatus childrenStatus = mergeNodesStatus(children);
		for (auto& subLevelStatus : childrenStatus)
		{
			output.push_back(subLevelStatus);
		}

		return output;
	}

	float GameStateNode::computeParentEvaluation(float currentEvaluation, unsigned nodePosition)
	{
		// If this node is the first of the list, we simply return the evaluation of the node, 
		// if not we compare it with the current evaluation of the parent and replace it only if we have a better one
		if (nodePosition > 0 && currentEvaluation > getNodeEvaluation())
		{
			return currentEvaluation;
		}
		return getNodeEvaluation();
	}
}