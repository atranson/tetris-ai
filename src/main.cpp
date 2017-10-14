#include <iostream>
#include <time.h> 
#include "GameState.h"
#include "GridView.h"
#include "DellacherieHeuristic.h"
#include "HeuristicStrategy.h"
#include <SFML/Graphics.hpp>

using namespace TetrisAI;

int main()
{
	srand(time(NULL));

	int height(20), width(10);

	int squareSize(20), innerBorder(1), outerBorder(2);
	sf::RenderWindow window(sf::VideoMode((squareSize + innerBorder) * width - innerBorder + outerBorder * 2, (squareSize + innerBorder) * height - innerBorder + outerBorder * 2), "Tetris AI");
	GridView gridView(sf::Vector2u(squareSize, squareSize), outerBorder, innerBorder, width, height);

	int polyominoSquares(4);
	std::vector<Polyomino> polyominos(Polyomino::getPolyominosList(polyominoSquares));
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

		int currentPieceIndex = rand() % polyominos.size();
		Polyomino currentPolyomino(polyominos[currentPieceIndex]);
		Transformation chosenMove(strategy.decideMove(gs, currentPolyomino));

		if (chosenMove.translation != -1)
		{
			gs.play(currentPolyomino, chosenMove);

			std::cout << "Chose (" << chosenMove.translation << ", " << chosenMove.rotation << ") for Piece #" << currentPieceIndex << std::endl;
			gridView.refreshGrid(gs.getGridContent());
			window.clear(sf::Color(255, 255, 255));
			window.draw(gridView);
		}
		else
		{
			std::cout << "Game over: could not place Piece #" << currentPieceIndex << std::endl;
		}
			
		window.display();

		system("pause");
	}

	return 0;
}