#pragma once
#include <SFML/Graphics.hpp>

enum TileType
{
	tile_Ground,
	tile_Wall,
	tile_Exit,
	tile_Platform,
	tile_Barrier,
	tile_Door,
};

class Tile
{
public:
	TileType m_TileType;

	sf::RectangleShape m_TileShape;
	sf::FloatRect* m_TileCollider;
	
	Tile(TileType _Type);
	~Tile();
};

