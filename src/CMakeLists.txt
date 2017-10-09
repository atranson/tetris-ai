add_library (TetrisAI 
	PolyominoState.cpp PolyominoState.h
)

add_executable (main main.cpp)
target_link_libraries (main TetrisAI)
