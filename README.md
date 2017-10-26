## About this project ##
As part of competitive entrance exams I took back in 2012, I worked on an artificial intelligence able to play Tetris. The result was a program made in OCaml (a functional language) which was based on Pierre Dellacherie's algorithm that relied on a simple heuristic.

This project is the result of the rewriting of the original OCaml program in C++. The environment in which the AI can play can be customized (different grid sizes, not only the 10x20 classic one) as well as the pieces (polyominos) that are to be played. Along with the usual tetraminos, other polyominos (dominos, triominos) can be played by the AI.

The AI as it is can consider multiple moves in advance, thus providing better results than the original algorithm of Pierre Dellacherie.

## Command line options ##
The following options are available when launching the main program

 - --height [-h] Height of the grid
 - --width [-w] Width of the grid
 - --polyomino [-p] Number of squares composing polyominos
 - --stepsAhead [-s] Number of polyominos known in advance (after the one currently being played)
 - --heuristicDepth [-d] Number of moves the decision tree should consider in advance
 - --noWindow Disable the window that displays the grid
 - --multithreading Enable multithreading for AI computations
