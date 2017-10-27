find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

include_directories (
	${TETRIS_AI_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)

add_definitions (-DBOOST_TEST_DYN_LINK)

add_executable (AIUnitTest 
	main.cpp
	PolyominoStateTest.cpp
	PolyominoTest.cpp
	GridTest.cpp
	UtilitiesTest.cpp
	DecisionTreeNodeTest.cpp
)
target_link_libraries (AIUnitTest
	TetrisAI
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
