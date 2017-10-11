cmake_minimum_required (VERSION 3.8)
project (TETRIS_AI)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

add_subdirectory (src) 
add_subdirectory (test)

enable_testing ()
add_test (NAME UnitTests COMMAND AIUnitTest)