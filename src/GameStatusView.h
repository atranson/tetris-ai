#ifndef TETRISAI_GAMESTATSVIEW_H
#define TETRISAI_GAMESTATSVIEW_H

#include <SFML/Graphics.hpp>
#include "GameSequence.h"

class GameStatusView : public sf::Drawable, public sf::Transformable {

public:
	GameStatusView(sf::Font& font, int width, int height) : font(font), width(width), height(height) {}
	void updateStatistics(TetrisAI::GameStatistics updatedStatistics);

private:
	int width, height;
	unsigned int polyominosPlayed;
	unsigned int linesCleared;
	sf::Font& font;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
