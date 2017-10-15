#ifndef TETRISAI_GRIDVIEW_H
#define TETRISAI_GRIDVIEW_H

#include <SFML/Graphics.hpp>

class GridView : public sf::Drawable, public sf::Transformable {
	
public:
	static const sf::Color outerBorderColor;
	static const sf::Color innerBorderColor;
	static const sf::Color fullTileColor;
	static const sf::Color emptyTileColor;

	GridView(sf::Vector2u tileSize, unsigned int outerBorderSize, unsigned int innerBorderSize, unsigned int width, unsigned int height);
	void refreshGrid(const std::vector<unsigned int>& gridContent);
	sf::Vector2u getBlockSize() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Vertex* getTileQuadPointer(unsigned int row, unsigned int col);

	sf::VertexArray vertices;
	unsigned int width;
	unsigned int height;
	unsigned int outerBorderSize;
	unsigned int innerBorderSize;

	sf::Vector2u drawableBlockSize;
};

#endif
