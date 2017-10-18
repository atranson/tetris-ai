#include <boost/test/unit_test.hpp>
#include "DecisionTreeNode.h"
#include "GameStateNode.h"
#include "PolyominoNode.h"

using namespace TetrisAI;

BOOST_AUTO_TEST_CASE(decision_tree_node_constructors_test) {
	GameState initialGameState(6, 6); // create basic game state
	std::vector<Polyomino> polyominos(Polyomino::getPolyominosList(3)); // retrieve the two possible triominos

	// BASIC CASE
	GameStateNode basicCase(initialGameState, 0, polyominos);
	DecisionTreeNode::NodeStatus basicCaseStatus(basicCase.getNodeStatus());
	BOOST_CHECK_EQUAL(basicCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(basicCaseStatus.size(), 1);
	
	// STEPS AHEAD = NUMBER OF KNOWN INCOMING POLYOMINOS
	initialGameState.addPolyominoToQueue(&(polyominos[0]));
	initialGameState.addPolyominoToQueue(&(polyominos[1]));
	GameStateNode complexCase(initialGameState, 1, polyominos);
	DecisionTreeNode::NodeStatus complexCaseStatus(complexCase.getNodeStatus());
	BOOST_CHECK_EQUAL(complexCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(complexCaseStatus[1]["GameStateNode"], 10);
	BOOST_CHECK_EQUAL(complexCaseStatus.size(), 2);

	GameStateNode complexCase2(initialGameState, 2, polyominos);
	DecisionTreeNode::NodeStatus complexCaseStatus2(complexCase2.getNodeStatus());
	BOOST_CHECK_EQUAL(complexCaseStatus2[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(complexCaseStatus2[1]["GameStateNode"], 10); // 10 possibilities for the I triomino
	BOOST_CHECK_EQUAL(complexCaseStatus2[2]["GameStateNode"], 200); // 20 possibilities for the L triomino
	BOOST_CHECK_EQUAL(complexCaseStatus2.size(), 3);

	// UNKNWON NEXT POLYOMINO CASE
	initialGameState = GameState(6, 6); // reset basic game state
	// This time we don't add any polyomino to the queue
	GameStateNode unknownCase(initialGameState, 1, polyominos);
	DecisionTreeNode::NodeStatus unknownCaseStatus(unknownCase.getNodeStatus());
	BOOST_CHECK_EQUAL(unknownCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(unknownCaseStatus[1]["PolyominoNode"], 2); // Layer that defines potential polyominos
	BOOST_CHECK_EQUAL(unknownCaseStatus[2]["GameStateNode"], 30); // 10+20 possibilities for the I triomino and L triomino combined
	BOOST_CHECK_EQUAL(unknownCaseStatus.size(), 3);

	GameStateNode unknownCase2(initialGameState, 2, polyominos);
	DecisionTreeNode::NodeStatus unknownCaseStatus2(unknownCase2.getNodeStatus());
	BOOST_CHECK_EQUAL(unknownCaseStatus2[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(unknownCaseStatus2[1]["PolyominoNode"], 2); // Layer that defines potential triominos (2)
	BOOST_CHECK_EQUAL(unknownCaseStatus2[2]["GameStateNode"], 30); // 10+20 possibilities for the I triomino and L triomino combined
	BOOST_CHECK_EQUAL(unknownCaseStatus2[3]["PolyominoNode"], 60); // Layer that defines potential triominos (2)
	BOOST_CHECK_EQUAL(unknownCaseStatus2[4]["GameStateNode"], 900); // (10+20)*Layer[N-2]
	BOOST_CHECK_EQUAL(unknownCaseStatus2.size(), 5);

	// MIXED AND HEAVIER CASE (FIRST INCOMING POLYOMINO ONLY IS KNOWN)
	initialGameState.addPolyominoToQueue(&(polyominos[0])); // Add one polyomino to the queue
	GameStateNode mixedCase(initialGameState, 3, polyominos);
	DecisionTreeNode::NodeStatus mixedCaseStatus(mixedCase.getNodeStatus());
	BOOST_CHECK_EQUAL(mixedCaseStatus[0]["GameStateNode"], 1);
	BOOST_CHECK_EQUAL(mixedCaseStatus[1]["GameStateNode"], 10); // 10 possibilities for the I triomino
	BOOST_CHECK_EQUAL(mixedCaseStatus[2]["PolyominoNode"], 20); // Layer that defines potential triominos (2)
	BOOST_CHECK_EQUAL(mixedCaseStatus[3]["GameStateNode"], 300); // (10+20)*Layer[N-2]
	BOOST_CHECK_EQUAL(mixedCaseStatus[4]["PolyominoNode"], 600); // Layer that defines potential triominos (2)
	BOOST_CHECK_EQUAL(mixedCaseStatus[5]["GameStateNode"], 9000); // (10+20)*Layer[N-2]
	BOOST_CHECK_EQUAL(mixedCaseStatus.size(), 6);
}