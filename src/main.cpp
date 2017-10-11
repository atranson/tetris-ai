#include <iostream>
#include <time.h> 
#include "GameState.h"
#include "DellacherieHeuristic.h"
#include "HeuristicStrategy.h"
#include <SFML/Graphics.hpp>

using namespace TetrisAI;

void initializeTetraminoList(std::vector<Polyomino>& list)
{
	if (list.size() != 0)
	{
		list.clear();
	}
	list.reserve(7);

	Polyomino T({ 2, 7 }), J({ 1, 7 }), L({ 4, 7 }), O({ 3, 3 }), S({ 6, 3 }), Z({3, 6}), I({ 15 });
	list.push_back(T);
	list.push_back(I);
	list.push_back(L);
	list.push_back(J);
	list.push_back(O);
	list.push_back(S);
	list.push_back(Z);
}

int main()
{
	srand(time(NULL));

	int height(20), width(10);

	int squareSize(20), innerBorder(1), outerBorder(2);
	sf::RenderWindow window(sf::VideoMode((squareSize + innerBorder) * width - innerBorder + outerBorder * 2, (squareSize + innerBorder) * height - innerBorder + outerBorder * 2), "Tetris AI");

	std::vector<Polyomino> tetraminos;
	initializeTetraminoList(tetraminos);
	GameState gs(width, height);
	DellacherieHeuristic chosenHeuristic;
	HeuristicStrategy strategy(chosenHeuristic);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		int currentPieceIndex = rand() % 7;
		Polyomino currentPolyomino(tetraminos[currentPieceIndex]);
		Transformation chosenMove(strategy.decideMove(gs, currentPolyomino));

		gs.play(currentPolyomino, chosenMove);
		std::cout << "Chose (" << chosenMove.translation << ", " << chosenMove.rotation << ") for Piece #" << currentPieceIndex << std::endl;
		system("pause"); 
		
		window.clear(sf::Color(255, 255, 255));
		window.display();
	}

	return 0;
}