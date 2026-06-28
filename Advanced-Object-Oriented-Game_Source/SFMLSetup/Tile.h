#pragma once
#include <SFML/Graphics.hpp>

enum TileType
{
	tile_Ground,
	tile_Wall,
	tile_Exit,
	tile_BreakBox,
	tile_Door,
};

class Tile
{
public:
	TileType m_TileType;

	sf::RectangleShape m_TileShape;
	sf::FloatRect* m_TileCollider;
	
	bool BoxBroken = false;
	
	Tile(TileType _Type);
	~Tile();
};

