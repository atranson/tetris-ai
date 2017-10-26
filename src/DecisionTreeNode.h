#ifndef TETRISAI_DECISIONTREENODE_H
#define TETRISAI_DECISIONTREENODE_H

#include "Polyomino.h"
#include "Heuristic.h"
#include <vector>
#include <map>
#include <memory>

namespace TetrisAI {

	class DecisionTreeNode {

	public:
		using NodeCount = std::map<std::string, int>;
		using NodeStatus = std::vector<NodeCount>;

		// Make sure implicitly declared destructors of concrete class are correctly called (important for freeing memory through unique_ptr destruction)
		virtual ~DecisionTreeNode() {};

		/// <summary>Update recursively the tree with a new polyomino</summary>
		/// <param name="newPolyomino">Polyomino that should enter the queue of known pending polyominos</param>
		/// <param name="depth">Number of moves that should be considered from this node</param>
		/// <param name="possiblePolyominos">List of potential polyominos to populate the decision tree if needed</param>
		/// <param name="possiblePolyominos">Heuristic that should be used to evaluate leaves and branches</param>
		/// <param name="useMultithreading">Toggle the use of multithreading to enhance the speed of decision making</param>
		virtual void updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic, bool useMultithreading) = 0;

		/// <summary>Returns true if the node is a PolyominoNode that consider the possibility that a certain polyomino will have to be played</summary>
		/// <param name="polyomino">Polyomino that should be matched</param>
		virtual bool matchPolyomino(Polyomino* polyomino) = 0;

		/// <summary>Helper method to move children from the current node to the given destination</summary>
		/// <param name="destination">Vector that should retrieve the ownership of the children of the current node</param>
		virtual void movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination) = 0;

		/// <summary>Find the child with the best evaluation and hand it over</summary>
		/// <returns>Unique pointer holding the best child ownership</returns>
		virtual std::unique_ptr<DecisionTreeNode> extractBestChild() = 0;

		/// <summary>Method that returns the structure of the tree (for debugging and testing purposes)</summary>
		virtual NodeStatus getNodeStatus() = 0;
		virtual bool isGameOver() const = 0;

		virtual float getNodeEvaluation() const = 0;
		virtual Transformation getPolyominoMove() const = 0;

		/// <summary>Computes the evaluation of the siblings as a whole (This method is meant to be called iteratively on all siblings at once)</summary>
		/// <param name="currentEvaluation">Evaluation of the siblings computed before reaching this node</param>
		/// <param name="nodePosition">Position of the node in the siblings collection</param>
		/// <returns>The evaluation of the siblings collection up to this node</returns>
		virtual float computeSiblingsEvaluation(float currentEvaluation, unsigned nodePosition) = 0;

	protected:
		NodeStatus mergeNodesStatus(std::vector<std::unique_ptr<DecisionTreeNode>>& nodes);
	};

}

#endif
