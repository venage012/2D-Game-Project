#include "Tile.h"

Tile::Tile(TileType _Type)
{
	m_TileType = _Type;
	m_TileShape.setSize({64,64});

	switch (_Type)
	{
	case tile_Ground:
		m_TileShape.setFillColor(sf::Color::Green);
		m_TileShape.setOutlineColor(sf::Color::Black);
		m_TileShape.setOutlineThickness(-.8);
		break;
	case tile_Wall:
		m_TileShape.setFillColor(sf::Color::White);
		m_TileShape.setOutlineColor(sf::Color::Black);
		m_TileShape.setOutlineThickness(-.8);
		break;
	case tile_Exit:
		m_TileShape.setFillColor(sf::Color::Yellow);
		m_TileShape.setOutlineColor(sf::Color::Black);
		m_TileShape.setOutlineThickness(-.8);
		break;
	case tile_BreakBox:
		m_TileShape.setFillColor(sf::Color::Cyan);
		m_TileShape.setOutlineColor(sf::Color::Black);
		m_TileShape.setOutlineThickness(-.8);
		break;
	case tile_Door:
		m_TileShape.setFillColor(sf::Color::Red);
		m_TileShape.setOutlineColor(sf::Color::Black);
		m_TileShape.setOutlineThickness(-.8);
		break;
	default:
		break;
	}

}

Tile::~Tile()
{
}
