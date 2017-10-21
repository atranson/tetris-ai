#include <iostream>
#include "GameSequence.h"
#include "GridView.h"
#include "GameStatusView.h"
#include "DellacherieHeuristic.h"
#include "HeuristicStrategy.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

using namespace TetrisAI;

void gameView(GameSequence& gameSequence, unsigned int refreshRate)
{
	sf::Font font;
	if (!font.loadFromFile("../res/Roboto-Regular.ttf"))
	{
		throw std::invalid_argument("Could not load font properly");
	}

	int squareSize(20), innerBorder(1), outerBorder(2);
	GridView gridView(sf::Vector2u(squareSize, squareSize), outerBorder, innerBorder, gameSequence.getGridWidth(), gameSequence.getGridHeight());
	GameStatusView statsView(font, gameSequence.getGridWidth(), gameSequence.getGridHeight());

	sf::Vector2u gridSize(gridView.getBlockSize());
	sf::RenderWindow window(sf::VideoMode(240 + gridSize.x, gridSize.y), "Tetris AI");
	statsView.setPosition(gridSize.x, 0);

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

		gridView.refreshGrid(gameSequence.getGameState().getGridContent());
		statsView.updateStatistics(gameSequence.getStats());
		window.clear(sf::Color(255, 255, 255));
		window.draw(gridView);
		window.draw(statsView);
		window.display();

		if (refreshRate < 1001)
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
	HeuristicStrategy strategy(chosenHeuristic, 2);
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