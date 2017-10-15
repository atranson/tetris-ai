#include "GridView.h"

const sf::Color GridView::outerBorderColor = sf::Color(0, 0, 0);
const sf::Color GridView::innerBorderColor = sf::Color(128, 128, 128);
const sf::Color GridView::fullTileColor = sf::Color(16, 16, 16);
const sf::Color GridView::emptyTileColor = sf::Color(255, 255, 255);

GridView::GridView(sf::Vector2u tileSize, unsigned int outerBorderSize, unsigned int innerBorderSize, unsigned int width, unsigned int height) : 
	width(width), height(height), outerBorderSize(outerBorderSize), innerBorderSize(innerBorderSize)
{
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(8 + width * height * 4); // We add eight vertices for the borders (two quads)

	// Borders vertices
	sf::Vertex* outerZone = &vertices[0];
	sf::Vertex* innerZone = &vertices[4];

	outerZone[0].position = sf::Vector2f(0, 0);
	outerZone[1].position = sf::Vector2f(outerBorderSize * 2 + width * (tileSize.x + innerBorderSize) - innerBorderSize, 0);
	outerZone[2].position = sf::Vector2f(outerBorderSize * 2 + width * (tileSize.x + innerBorderSize) - innerBorderSize, outerBorderSize * 2 + height * (tileSize.y + innerBorderSize) - innerBorderSize);
	outerZone[3].position = sf::Vector2f(0, outerBorderSize * 2 + height * (tileSize.y + innerBorderSize) - innerBorderSize);

	innerZone[0].position = sf::Vector2f(outerBorderSize, outerBorderSize);
	innerZone[1].position = sf::Vector2f(outerBorderSize + width * (tileSize.x + innerBorderSize) - innerBorderSize, outerBorderSize);
	innerZone[2].position = sf::Vector2f(outerBorderSize + width * (tileSize.x + innerBorderSize) - innerBorderSize, outerBorderSize + height * (tileSize.y + innerBorderSize) - innerBorderSize);
	innerZone[3].position = sf::Vector2f(outerBorderSize, outerBorderSize + height * (tileSize.y + innerBorderSize) - innerBorderSize);

	for (int i = 0; i < 4; i++)
	{
		outerZone[i].color = outerBorderColor;
		innerZone[i].color = innerBorderColor;
	}

	// We invert the position so that lower bits represents tiles on the right
	// and so that the row numbered with 0 represents the bottom row in the grid
	for (unsigned int row = 0; row < height; row++)
	{
		int swappedYIndex = height - 1 - row;
		for (unsigned int col = 0; col < width; col++)
		{
			// retrieve a pointer of the quad we need to define
			sf::Vertex* quad = getTileQuadPointer(row, col);

			int swappedXIndex = width - 1 - col;
			// define the position of each of its corners
			quad[0].position = sf::Vector2f(outerBorderSize + swappedXIndex * (tileSize.x + innerBorderSize), outerBorderSize + swappedYIndex * (tileSize.y + innerBorderSize));
			quad[1].position = sf::Vector2f(outerBorderSize + (swappedXIndex + 1) * (tileSize.x + innerBorderSize) - innerBorderSize, outerBorderSize + swappedYIndex * (tileSize.y + innerBorderSize));
			quad[2].position = sf::Vector2f(outerBorderSize + (swappedXIndex + 1) * (tileSize.x + innerBorderSize) - innerBorderSize, outerBorderSize + (swappedYIndex + 1) * (tileSize.y + innerBorderSize) - innerBorderSize);
			quad[3].position = sf::Vector2f(outerBorderSize + swappedXIndex * (tileSize.x + innerBorderSize), outerBorderSize + (swappedYIndex + 1) * (tileSize.y + innerBorderSize) - innerBorderSize);

			// Default color is white (empty tile)
			for (int i = 0; i < 4; i++)
			{
				quad[i].color = emptyTileColor;
			}
		}
	}

	drawableBlockSize = sf::Vector2u(
		(tileSize.x + innerBorderSize) * width - innerBorderSize + outerBorderSize * 2,
		(tileSize.y + innerBorderSize) * height - innerBorderSize + outerBorderSize * 2
	);
}

sf::Vector2u GridView::getBlockSize() const
{
	return drawableBlockSize;
}

void GridView::refreshGrid(const std::vector<unsigned int>& gridContent)
{
	unsigned int maxRow = (gridContent.size() > height) ? height : gridContent.size();
	for (unsigned int row = 0; row < maxRow; row++)
	{
		unsigned int rowValue = gridContent[row];
		for (unsigned int col = 0; col < width; col++)
		{
			// retrieve a pointer of the quad we need to update
			sf::Vertex* quad = getTileQuadPointer(row, col);

			sf::Color fillColor = (rowValue & 1) ? fullTileColor : emptyTileColor;
			for (int i = 0; i < 4; i++)
			{
				quad[i].color = fillColor;
			}

			rowValue >>= 1;
		}
	}
}

void GridView::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(vertices, states);
}

sf::Vertex* GridView::getTileQuadPointer(unsigned int row, unsigned int col)
{
	return &vertices[8 + (row * width + col) * 4]; // Offset of 8 due to the two quads defining the borders
}
