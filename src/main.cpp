#include <boost/program_options.hpp>
#include <iostream>
#include "GameSequence.h"
#include "GridView.h"
#include "Grid.h"
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

	unsigned short squareSize(20), innerBorder(1), outerBorder(2);
	GridView gridView(sf::Vector2<unsigned short>(squareSize, squareSize), outerBorder, innerBorder, gameSequence.getGridWidth(), gameSequence.getGridHeight());
	GameStatusView statsView(font, gameSequence.getGridWidth(), gameSequence.getGridHeight());

	sf::Vector2u gridSize(gridView.getBlockSize());
	sf::RenderWindow window(sf::VideoMode(240 + gridSize.x, gridSize.y), "Tetris AI");
	statsView.setPosition((float)gridSize.x, 0);

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

int main(int argc, char* argv[])
{
	int height(20), width(10), polyominoSquares(4);
	unsigned stepsAhead(0), heuristicDepth(1);
	bool noWindow(false), useMultithreading(false);

	// PARSING PROGRAM OPTIONS
	namespace po = boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("height,h", po::value<int>()->default_value(height), "set height of the grid [4-32]")
		("width,w", po::value<int>()->default_value(width), "set width of the grid [4-32]")
		("polyomino,p", po::value<int>()->default_value(polyominoSquares), "set the number of squares composing polyominos [1-5]")
		("stepsAhead,s", po::value<unsigned int>()->default_value(stepsAhead), "set the number of polyominos known in advance (excepting the one currently being played) [0-5]")
		("heuristicDepth,d", po::value<unsigned int>()->default_value(heuristicDepth), "set the number of moves the decision tree should consider in advance [1-4]")
		("noWindow", po::bool_switch(&noWindow), "disable the window that displays the grid")
		("multithreading", po::bool_switch(&useMultithreading), "enable multithreading for AI computations")
		;

	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << "\n";
			return 1;
		}

		if (vm.count("height")) { height = vm["height"].as<int>(); }
		if (vm.count("width")) { width = vm["width"].as<int>(); }
		if (vm.count("polyomino")) { polyominoSquares = vm["polyomino"].as<int>(); }
		if (vm.count("stepsAhead")) { stepsAhead = vm["stepsAhead"].as<unsigned int>(); }
		if (vm.count("heuristicDepth")) { heuristicDepth = vm["heuristicDepth"].as<unsigned int>(); }

		// Checking values ranges
		if (height < Grid::minSize || height > Grid::maxSize || width < Grid::minSize || width > Grid::maxSize)
		{
			std::cout << "Grid size out of range [" << Grid::minSize << "-" << Grid::maxSize << "]" << std::endl;
			return 1;
		}
		if (polyominoSquares < 1 || polyominoSquares > Polyomino::maxSquares)
		{
			std::cout << "Polyomino size out of range [1-" << Polyomino::maxSquares << "]" << std::endl;
			return 1;
		}
		if (stepsAhead < 0 || stepsAhead > GameSequence::maxStepsAhead)
		{
			std::cout << "Steps ahead parameter out of range [0-" << GameSequence::maxStepsAhead << "]" << std::endl;
			return 1;
		}
		if (heuristicDepth < 1 || heuristicDepth > HeuristicStrategy::maxDepth)
		{
			std::cout << "Heuristic depth parameter out of range [1-" << HeuristicStrategy::maxDepth << "]" << std::endl;
			return 1;
		}

		// Reporting launch setup
		std::cout << "Launching game sequence with the following parameters:" << std::endl
			<< "\tGrid(w x h) " << width << "x" << height << std::endl
			<< "\tPolyomino will be composed of " << polyominoSquares << " squares" << std::endl
			<< "\t" << stepsAhead << " polyomino(s) will be known in advance during play" << std::endl
			<< "\tThe AI will consider " << heuristicDepth << " move(s) in advance (including the current polyomino)" << std::endl;

	}
	catch (po::error& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		return 1;
	}

	// MAIN PROGRAM
	DellacherieHeuristic chosenHeuristic;
	std::shared_ptr<AIStrategy> strategy(std::make_shared<HeuristicStrategy>(chosenHeuristic, heuristicDepth, useMultithreading));
	GameSequence gameSequence(width, height, polyominoSquares, strategy, stepsAhead);

	std::vector<std::thread> threads;
	threads.push_back(std::thread(&GameSequence::playGame, &gameSequence));
	if (!noWindow)
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