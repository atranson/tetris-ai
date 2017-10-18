add_library (TetrisAI 
	PolyominoState.cpp PolyominoState.h
	Polyomino.cpp Polyomino.h
	MoveResult.h
	GridTools.cpp GridTools.h
	Grid.cpp Grid.h
	GameState.cpp GameState.h
	AIStrategy.h
	Heuristic.cpp Heuristic.h
	DellacherieHeuristic.cpp DellacherieHeuristic.h
	HeuristicStrategy.cpp HeuristicStrategy.h
	DecisionTreeNode.cpp DecisionTreeNode.h
	GameStateNode.cpp GameStateNode.h
	PolyominoNode.cpp PolyominoNode.h
	GameSequence.cpp GameSequence.h
)

configure_file(../res/Roboto-Regular.ttf res/Roboto-Regular.ttf COPYONLY)

add_executable (main 
	main.cpp 
	GridView.cpp GridView.h 
	GameStatusView.cpp GameStatusView.h
)

#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(main TetrisAI ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()
