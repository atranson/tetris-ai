#include "GameStatusView.h"
#include <string> 

void GameStatusView::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color::Black);

	std::string gridText;
	gridText += "GRID " + std::to_string(width) + "x" + std::to_string(height);
	text.setString(gridText);
	text.setCharacterSize(10);
	text.setPosition(10, 10);
	target.draw(text, states);

	text.setString("MOVES");
	text.setCharacterSize(10);
	text.setPosition(10, 30);
	target.draw(text, states);

	text.setString(std::to_string(polyominosPlayed));
	text.setCharacterSize(16);
	text.setPosition(10, 42);
	target.draw(text, states);

	text.setString("LINES");
	text.setCharacterSize(10);
	text.setPosition(10, 68);
	target.draw(text, states);

	text.setString(std::to_string(linesCleared));
	text.setCharacterSize(16);
	text.setPosition(10, 80);
	target.draw(text, states);
}

void GameStatusView::updateStatistics(TetrisAI::GameStatistics updatedStatistics)
{
	polyominosPlayed = updatedStatistics.polyominosPlayed;
	linesCleared = updatedStatistics.linesCleared;
}