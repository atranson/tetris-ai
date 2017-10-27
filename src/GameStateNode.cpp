#include "GameStateNode.h"
#include "PolyominoNode.h"
#include <stdexcept>
#include <thread>
#include "Utilities.h"

namespace TetrisAI {

	GameStateNode::GameStateNode(const GameState& gameState, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic) : gameState(gameState) 
	{ 
		// We first build the children (which will trigger their own evaluation)
		buildChildren(depth, possiblePolyominos, heuristic); 
		// Then we compute the evaluation of the node
		updateNodeEvaluation(heuristic);
	}

	void GameStateNode::buildChildren(int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic)
	{
		// If it a game over, we can't build anything from there
		// If depth is 0, we don't have anything to build, just to evaluate the current state
		if (depth <= 0 || gameState.isGameOver())
		{
			return;
		}

		// If the list of children is not empty, we clear it beforehand
		if (!children.empty())
		{
			children.clear();
		}

		GameState newBaseGameState = gameState; // Copy game state
		Polyomino* comingPolyomino(newBaseGameState.polyominoQueueHead()); // Retrieve next polyomino
		// If the queue was empty, we don't know what's next
		if (comingPolyomino == nullptr)
		{
			// We consider every possible polyomino and create a subtree for each one of them
			children.reserve(possiblePolyominos.size());
			for (auto& p : possiblePolyominos)
			{
				children.push_back(std::make_unique<PolyominoNode>(newBaseGameState, &p, depth, possiblePolyominos, heuristic));
			}
		}
		else
		{
			// If not, we know what polyomino we have to consider
			Transformation t;
			int gridWidth(newBaseGameState.getGrid().getWidth());
			// Test each possible move for the given polyomino and keep the one that has the best evaluation
			for (t.rotation = 0; t.rotation < comingPolyomino->getRotationCount(); t.rotation++)
			{
				int rotatedPolyominoWidth(comingPolyomino->getRotatedPiece(t.rotation).getWidth());
				for (t.translation = 0; t.translation <= gridWidth - rotatedPolyominoWidth; t.translation++)
				{
					// Create a new game state where that move was played
					GameState postMoveState = newBaseGameState;
					postMoveState.play(t);

					children.push_back(std::make_unique<GameStateNode>(postMoveState, depth - 1, possiblePolyominos, heuristic));
				}
			}
		}
	}

	void GameStateNode::updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic, bool useMultithreading)
	{
		if (depth <= 0)
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
			// If the node has no children, we simply have to build them
			buildChildren(depth, possiblePolyominos, heuristic);
		}
		else
		{
			// If the next polyomino was unknown before and is known now
			if (queueSizeBeforeUpdate == 0 && newPolyomino != nullptr)
			{
				if (!trimBranches(newPolyomino))
				{
					throw std::runtime_error("Error:  could not find a match for a certain polyomino in the tree decision. Tree state unexpected. This layer should be composed of PolyominoNodes with one for each possible polyomino.");
				}
				newPolyomino = nullptr; // Polyomino has been "used" at this level of depth
			}

			// For the next step, we recursively call updateTree on the children of this node
			if (useMultithreading)
			{
				std::vector<int> splitRangesBounds(splitRange(0, children.size(), std::thread::hardware_concurrency()));
				
				// Split up the work between a number of sub threads
				std::vector<std::thread> subThreads;
				subThreads.reserve(splitRangesBounds.size() - 1);
				for (unsigned i = 0; i < splitRangesBounds.size() - 1; i++)
				{
					subThreads.push_back(std::thread(&GameStateNode::updateSubTree, this,
						splitRangesBounds[i], splitRangesBounds[i+1] - 1,
						newPolyomino, depth, std::ref(possiblePolyominos), std::ref(heuristic)
					));
				}

				// Wait for each thread to finish
				for (auto& t : subThreads)
				{
					t.join();
				}
			}
			else
			{
				updateSubTree(0, children.size() - 1, newPolyomino, depth, possiblePolyominos, heuristic);
			}
		}

		// Finally, we update the node evaluation since its children have been updated as well
		updateNodeEvaluation(heuristic);
	}

	void GameStateNode::updateSubTree(unsigned from, unsigned to, Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic)
	{
		for (unsigned i = from; i <= to; i++)
		{
			children[i]->updateTree(newPolyomino, depth - 1, possiblePolyominos, heuristic, false);
		}
	}

	bool GameStateNode::trimBranches(Polyomino* matchingPolyomino)
	{
		// Search among children for a PolyominoNode that considered the case where "matchingPolyomino" is played
		std::unique_ptr<DecisionTreeNode> matchingChild(nullptr);
		while (!children.empty() && matchingChild == nullptr)
		{
			if (children.back()->matchPolyomino(matchingPolyomino))
			{
				matchingChild = std::move(children.back());
			}
			children.pop_back();
		}

		if (matchingChild == nullptr)
		{
			return false;
		}

		// We then replace this node's children by the children of the PolyominoNode that matched
		children.clear();
		matchingChild->movingChildrenOwnership(children);
		return true;
	}

	void GameStateNode::updateNodeEvaluation(Heuristic& heuristic)
	{
		if (children.empty())
		{
			nodeEvaluation = heuristic.evaluate(gameState);
		}
		else
		{
			// Update the evaluation of the node based on its children
			float childrenEvaluation(0); // Initialize the evaluation of the node
			int childNodePosition(0);
			for (auto& child : children)
			{
				childrenEvaluation = child->computeSiblingsEvaluation(childrenEvaluation, childNodePosition);
				childNodePosition++;
			}
			nodeEvaluation = heuristic.evaluateBranch(gameState, childrenEvaluation);
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
		for (unsigned i = 1; i < children.size(); i++)
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

	float GameStateNode::getNodeEvaluation() const 
	{ 
		return nodeEvaluation; 
	}

	Transformation GameStateNode::getPolyominoMove() const 
	{ 
		return gameState.getPolyominoMove(); 
	}

	bool GameStateNode::isGameOver() const
	{
		return gameState.isGameOver();
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

	float GameStateNode::computeSiblingsEvaluation(float currentEvaluation, unsigned nodePosition)
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