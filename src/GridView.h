#ifndef TETRISAI_GRIDVIEW_H
#define TETRISAI_GRIDVIEW_H

#include <SFML/Graphics.hpp>

class GridView : public sf::Drawable, public sf::Transformable {
	
public:
	static const sf::Color outerBorderColor;
	static const sf::Color innerBorderColor;
	static const sf::Color fullTileColor;
	static const sf::Color emptyTileColor;

	GridView(sf::Vector2<unsigned short> tileSize, unsigned short outerBorderSize, unsigned short innerBorderSize, unsigned short width, unsigned short height);
	void refreshGrid(const std::vector<unsigned int>& gridContent);
	sf::Vector2u getBlockSize() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Vertex* getTileQuadPointer(unsigned int row, unsigned int col);

	sf::VertexArray vertices;
	unsigned short width;
	unsigned short height;
	unsigned short outerBorderSize;
	unsigned short innerBorderSize;

	sf::Vector2u drawableBlockSize;
};

#endif
