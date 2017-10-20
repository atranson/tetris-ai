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

		virtual void updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic) = 0;
		virtual bool matchPolyomino(Polyomino* polyomino) = 0;
		virtual void movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination) = 0;
		virtual std::unique_ptr<DecisionTreeNode> extractBestChild() = 0;
		virtual NodeStatus getNodeStatus() = 0;
		virtual float getNodeEvaluation() const = 0;
		virtual float computeParentEvaluation(float currentEvaluation, unsigned nodePosition) = 0;

	protected:
		NodeStatus mergeNodesStatus(std::vector<std::unique_ptr<DecisionTreeNode>>& nodes);
	};

}

#endif
