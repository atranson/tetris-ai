add_library (TetrisAI 
	PolyominoState.cpp PolyominoState.h
	Polyomino.cpp Polyomino.h
	MoveResult.h
	GridTools.cpp GridTools.h
	Grid.cpp Grid.h
)

add_executable (main main.cpp)
target_link_libraries (main TetrisAI)
