#include "HeuristicStrategy.h"
#include "GameStateNode.h"
#include <stdexcept>

namespace TetrisAI {

	HeuristicStrategy::HeuristicStrategy(Heuristic& heuristic, unsigned int depth, bool useMultithreading) :
		heuristic(heuristic), depth(depth), decisionTreeRoot(nullptr), useMultithreading(useMultithreading)
	{
		if (depth > maxDepth)
		{
			throw std::invalid_argument("This implementation does not allow decision trees to consider more than 4 moves at once.");
		}
	}

	Transformation HeuristicStrategy::decideMove(const GameState& gs, std::vector<Polyomino>& possiblePolyominos)
	{
		// If the tree has not been initialized
		if (!decisionTreeRoot)
		{
			initializeTree(gs, possiblePolyominos);
		}
		else
		{
			Polyomino* lastAddedPolyomino(gs.polyominoQueueTail());
			if (lastAddedPolyomino != nullptr)
			{
				// Update tree content and nodes evaluation by building new level if necessary
				decisionTreeRoot->updateTree(lastAddedPolyomino, depth, possiblePolyominos, heuristic, useMultithreading);	
			}
			else
			{
				throw std::invalid_argument("Given game state does not have any pending polyomino");
			}
		}

		// Replace the root by its best child (trigger deletion of siblings and their subtrees)
		decisionTreeRoot = decisionTreeRoot->extractBestChild();

		if (decisionTreeRoot->isGameOver())
		{
			return Transformation(-1, -1);
		}
		return decisionTreeRoot->getPolyominoMove();
	}

	void HeuristicStrategy::initializeTree(const GameState& gs, std::vector<Polyomino>& possiblePolyominos)
	{
		decisionTreeRoot = std::make_unique<GameStateNode>(gs, depth, possiblePolyominos, heuristic);
	}

}