add_library (TetrisAI 
	PolyominoState.cpp PolyominoState.h
	Polyomino.cpp Polyomino.h
	MoveResult.h
	Utilities.cpp Utilities.h
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

add_definitions(-DBOOST_ALL_NO_LIB)

configure_file(../res/Roboto-Regular.ttf res/Roboto-Regular.ttf COPYONLY)

add_executable (main 
	main.cpp 
	GridView.cpp GridView.h 
	GameStatusView.cpp GameStatusView.h
)

#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED graphics window system)

set(Boost_USE_STATIC_LIBS   ON)
find_package (Boost COMPONENTS program_options REQUIRED)

if(SFML_FOUND)
  include_directories(
	${SFML_INCLUDE_DIR} 
	${Boost_INCLUDE_DIRS}
  )
  target_link_libraries(main 
	TetrisAI 
	${SFML_LIBRARIES} 
	${SFML_DEPENDENCIES}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
  )
endif()
