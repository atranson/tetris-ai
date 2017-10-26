#ifndef TETRISAI_GAMESTATENODE_H
#define TETRISAI_GAMESTATENODE_H

#include "DecisionTreeNode.h"
#include "GameState.h"
#include <memory>
#include <vector>

namespace TetrisAI {

	class GameStateNode : public DecisionTreeNode {

	public:
		GameStateNode(const GameState& gameState, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic);
		virtual void updateTree(Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic, bool useMultithreading);
		virtual void movingChildrenOwnership(std::vector<std::unique_ptr<DecisionTreeNode>>& destination);
		virtual std::unique_ptr<DecisionTreeNode> extractBestChild();
		virtual bool matchPolyomino(Polyomino* polyomino);
		virtual NodeStatus getNodeStatus();
		virtual float getNodeEvaluation() const;
		virtual Transformation getPolyominoMove() const;
		virtual bool isGameOver() const;
		virtual float computeSiblingsEvaluation(float currentEvaluation, unsigned nodePosition);

	private:
		float nodeEvaluation;
		GameState gameState;
		/// <summary>Holds an homogeneous collection (by construction) of concrete DecisionTreeNodes</summary>
		std::vector<std::unique_ptr<DecisionTreeNode>> children;

		void buildChildren(int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic);
		void updateNodeEvaluation(Heuristic& heuristic);

		/// <summary>Call updateTree on a subset of children</summary>
		/// <param name="from">Index of the first child</param>
		/// <param name="to">Index of the last child</param>
		void updateSubTree(unsigned from, unsigned to, Polyomino* newPolyomino, int depth, std::vector<Polyomino>& possiblePolyominos, Heuristic& heuristic);
	};

}

#endif
