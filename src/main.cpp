#include <iostream>
#include "GameSequence.h"
#include "GridView.h"
#include "DellacherieHeuristic.h"
#include "HeuristicStrategy.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

using namespace TetrisAI;

void gameView(GameSequence& gameSequence, unsigned int refreshRate)
{
	int squareSize(20), innerBorder(1), outerBorder(2);
	sf::RenderWindow window(sf::VideoMode((squareSize + innerBorder) * gameSequence.getGridWidth() - innerBorder + outerBorder * 2, (squareSize + innerBorder) * gameSequence.getGridHeight() - innerBorder + outerBorder * 2), "Tetris AI");
	GridView gridView(sf::Vector2u(squareSize, squareSize), outerBorder, innerBorder, gameSequence.getGridWidth(), gameSequence.getGridHeight());

	while (window.isOpen() && gameSequence.getStatus() != GameSequence::Status::GameOver)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		gridView.refreshGrid(gameSequence.getGameState().getGridContent());
		window.clear(sf::Color(255, 255, 255));
		window.draw(gridView);
		window.display();

		if (refreshRate <= 1001)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / refreshRate));
		}
	}
}

int main()
{
	int height(20), width(10);
	bool enableView(true);

	int polyominoSquares(4);
	DellacherieHeuristic chosenHeuristic;
	HeuristicStrategy strategy(chosenHeuristic);
	GameSequence gameSequence(width, height, polyominoSquares, &strategy);

	std::vector<std::thread> threads;
	threads.push_back(std::thread(&GameSequence::playGame, &gameSequence));
	if (enableView)
	{
		threads.push_back(std::thread(gameView, std::ref(gameSequence), 33));
	}

	for (auto& t : threads)
	{
		t.join();
	}

	system("pause");
	return 0;
}