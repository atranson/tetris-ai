#include <boost/test/unit_test.hpp>
#include <memory>
#include "DecisionTreeNode.h"
#include "GameStateNode.h"
#include "PolyominoNode.h"
#include "Heuristic.h"

using namespace TetrisAI;

class MockHeuristic : public Heuristic {
	private:
		virtual float evaluate(const GameState& gs, float stopValue, bool useStopValue)
		{
			float output(0);
			const std::vector<unsigned> grid(gs.getGrid().getContent());
			for (auto& row : grid)
			{
				output -= row;
			}
			return output;
		}
};

BOOST_AUTO_TEST_CASE(decision_tree_node_constructors_test) {
	GameState initialGameState(6, 6); // create basic game state
	std::vector<Polyomino> triominos(Polyomino::getPolyominosList(3)); // retrieve the two possible triominos
	MockHeuristic heuristic;

	// BASIC CASE
	GameStateNode basicCase(initialGameState, 0, triominos, heuristic);
	DecisionTreeNode::NodeStatus basicCaseStatus(basicCase.getNodeStatus());
	BOOST_CHECK_EQUAL(basicCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(basicCaseStatus.size(), 1);
	BOOST_CHECK_EQUAL(basicCase.getNodeEvaluation(), 0);
	
	// STEPS AHEAD = NUMBER OF KNOWN INCOMING POLYOMINOS
	initialGameState.addPolyominoToQueue(&(triominos[0]));
	initialGameState.addPolyominoToQueue(&(triominos[1]));
	std::unique_ptr<DecisionTreeNode> complexCase(std::make_unique<GameStateNode>(initialGameState, 1, triominos, heuristic));
	DecisionTreeNode::NodeStatus complexCaseStatus(complexCase->getNodeStatus());
	BOOST_CHECK_EQUAL(complexCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(complexCaseStatus[1]["GameStateNode"], 10); // 10 possibilities for the I triomino
	BOOST_CHECK_EQUAL(complexCaseStatus.size(), 2);
	// Best case for MockHeuristic is when the I is put on the right of the grid thus accounting for -1 3 times
	BOOST_CHECK_EQUAL(complexCase->getNodeEvaluation(), -3); 
	// If we extract the best child that produced that evaluation, we should get the corresponding transformation
	complexCase = complexCase->extractBestChild();
	BOOST_CHECK_EQUAL(complexCase->getPolyominoMove().translation, 0);
	BOOST_CHECK_EQUAL(complexCase->getPolyominoMove().rotation, 1);
	BOOST_CHECK_EQUAL(complexCase->getNodeEvaluation(), -3);
	
	GameStateNode complexCase2(initialGameState, 2, triominos, heuristic);
	DecisionTreeNode::NodeStatus complexCaseStatus2(complexCase2.getNodeStatus());
	BOOST_CHECK_EQUAL(complexCaseStatus2[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(complexCaseStatus2[1]["GameStateNode"], 10); // 10 possibilities for the I triomino
	BOOST_CHECK_EQUAL(complexCaseStatus2[2]["GameStateNode"], 200); // 20 possibilities for the L triomino
	BOOST_CHECK_EQUAL(complexCaseStatus2.size(), 3);
	// Best case for MockHeuristic is when the I and L are put on the right of the grid thus accounting for -1 4 times and -3 once
	BOOST_CHECK_EQUAL(complexCase2.getNodeEvaluation(), -7); 
	
	// UNKNWON NEXT POLYOMINO CASE
	initialGameState = GameState(6, 6); // reset basic game state
	// This time we don't add any polyomino to the queue
	GameStateNode unknownCase(initialGameState, 1, triominos, heuristic);
	DecisionTreeNode::NodeStatus unknownCaseStatus(unknownCase.getNodeStatus());
	BOOST_CHECK_EQUAL(unknownCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(unknownCaseStatus[1]["PolyominoNode"], 2); // Layer that defines potential triominos
	BOOST_CHECK_EQUAL(unknownCaseStatus[2]["GameStateNode"], 30); // 10+20 possibilities for the I triomino and L triomino combined
	BOOST_CHECK_EQUAL(unknownCaseStatus.size(), 3);
	// Best case for MockHeuristic is the mean of the best case for a I (-3) and for a L (-4)
	BOOST_CHECK_EQUAL(unknownCase.getNodeEvaluation(), -3.5);

	
	GameStateNode unknownCase2(initialGameState, 2, triominos, heuristic);
	DecisionTreeNode::NodeStatus unknownCaseStatus2(unknownCase2.getNodeStatus());
	BOOST_CHECK_EQUAL(unknownCaseStatus2[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(unknownCaseStatus2[1]["PolyominoNode"], 2); // Layer that defines potential triominos (2)
	BOOST_CHECK_EQUAL(unknownCaseStatus2[2]["GameStateNode"], 30); // 10+20 possibilities for the I triomino and L triomino combined
	BOOST_CHECK_EQUAL(unknownCaseStatus2[3]["PolyominoNode"], 60); // Layer that defines potential triominos (2)
	BOOST_CHECK_EQUAL(unknownCaseStatus2[4]["GameStateNode"], 900); // (10+20)*Layer[N-2]
	BOOST_CHECK_EQUAL(unknownCaseStatus2.size(), 5);
	
	// MIXED AND HEAVIER CASE (FIRST INCOMING POLYOMINO ONLY IS KNOWN)
	initialGameState.addPolyominoToQueue(&(triominos[0])); // Add one polyomino to the queue
	GameStateNode mixedCase(initialGameState, 3, triominos, heuristic);
	DecisionTreeNode::NodeStatus mixedCaseStatus(mixedCase.getNodeStatus());
	BOOST_CHECK_EQUAL(mixedCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(mixedCaseStatus[1]["GameStateNode"], 10); // 10 possibilities for the I triomino
	BOOST_CHECK_EQUAL(mixedCaseStatus[2]["PolyominoNode"], 20); // Layer that defines potential triominos (2)
	BOOST_CHECK_EQUAL(mixedCaseStatus[3]["GameStateNode"], 300); // (10+20)*Layer[N-2]
	BOOST_CHECK_EQUAL(mixedCaseStatus[4]["PolyominoNode"], 600); // Layer that defines potential triominos (2)
	BOOST_CHECK_EQUAL(mixedCaseStatus[5]["GameStateNode"], 9000); // (10+20)*Layer[N-2]
	BOOST_CHECK_EQUAL(mixedCaseStatus.size(), 6);
}

// Complementary test to verify behavior in mixed case
BOOST_AUTO_TEST_CASE(decision_tree_node_evaluation_test) {
	GameState initialGameState(4, 10); // create game state with the smallest width possible
	std::vector<Polyomino> triominos(Polyomino::getPolyominosList(3)); // retrieve the two possible triominos
	MockHeuristic heuristic;
	initialGameState.addPolyominoToQueue(&(triominos[0]));

	GameStateNode mixedCase(initialGameState, 2, triominos, heuristic);
	DecisionTreeNode::NodeStatus mixedCaseStatus(mixedCase.getNodeStatus());
	BOOST_CHECK_EQUAL(mixedCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(mixedCaseStatus[1]["GameStateNode"], 6); // 6 possibilities for the I in this grid
	BOOST_CHECK_EQUAL(mixedCaseStatus[2]["PolyominoNode"], 12); // 2 triominos possibles
	BOOST_CHECK_EQUAL(mixedCaseStatus[3]["GameStateNode"], 108); // (6+12)*Layer[N-2] > 6 possibilities for I, 12 for L in grid of width 4
	BOOST_CHECK_EQUAL(mixedCaseStatus.size(), 4);
	// Best case for MockHeuristic obtained for the branch where I is laid horizontally on the left of the grid
	// Best subcases of that branch (with MockHeuristic) respectively output -2 and -3 for the two possible triominos: I and L
	// Thus we get a mean of -2.5 for this branch which is the best one there is from an empty grid and starting with an I
	BOOST_CHECK_EQUAL(mixedCase.getNodeEvaluation(), -2.5);
}

BOOST_AUTO_TEST_CASE(decision_tree_node_update_tree_tree) {
	GameState initialGameState(4, 10); // create game state with the smallest width possible
	std::vector<Polyomino> triominos(Polyomino::getPolyominosList(3)); // retrieve the two possible triominos
	MockHeuristic heuristic;
	initialGameState.addPolyominoToQueue(&(triominos[0]));

	int depth(2);
	std::unique_ptr<DecisionTreeNode> mixedCase(std::make_unique<GameStateNode>(initialGameState, depth, triominos, heuristic));

	// Replace the root by its best child (trigger deletion of siblings and their subtrees)
	mixedCase = mixedCase->extractBestChild();
	DecisionTreeNode::NodeStatus mixedCaseStatus(mixedCase->getNodeStatus());
	BOOST_CHECK_EQUAL(mixedCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(mixedCaseStatus[1]["PolyominoNode"], 2); // 2 triominos possibles
	BOOST_CHECK_EQUAL(mixedCaseStatus[2]["GameStateNode"], 18); // (6+12)*Layer[N-2] > 6 possibilities for I, 12 for L in grid of width 4
	BOOST_CHECK_EQUAL(mixedCaseStatus.size(), 3);
	BOOST_CHECK_EQUAL(mixedCase->getNodeEvaluation(), -2.5);

	// Build the next level of three by adding a new polyomino in the queue
	mixedCase->updateTree(&(triominos[1]), depth, triominos, heuristic);
	mixedCaseStatus = mixedCase->getNodeStatus();
	BOOST_CHECK_EQUAL(mixedCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(mixedCaseStatus[1]["GameStateNode"], 12); // 12 possibilities for the L in this grid
	BOOST_CHECK_EQUAL(mixedCaseStatus[2]["PolyominoNode"], 24); // 2 triominos possibles
	BOOST_CHECK_EQUAL(mixedCaseStatus[3]["GameStateNode"], 216); // (6+12)*Layer[N-2] > 6 possibilities for I, 12 for L in grid of width 4
	BOOST_CHECK_EQUAL(mixedCaseStatus.size(), 4);
	BOOST_CHECK(mixedCase->getNodeEvaluation() != -2.5); // Should have been updated
}