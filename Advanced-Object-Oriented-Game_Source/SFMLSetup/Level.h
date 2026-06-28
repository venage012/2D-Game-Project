#pragma once
#include <string>
#include <fstream>
#include "Character.h"

enum CurrentLevel
{
	Level_1,
	Level_2,
	Level_3,
	Level_Test
};

class Level
{
public:
	// level size definitions
	static const int m_LevelWidth = 100;
	static const int m_LevelHeight = 100;

	// all the tiles that dont collide but are visible. 
	std::vector<Tile*> m_LevelTiles;

	// all the collidable tiles
	std::vector<Tile*> m_LevelWallTiles;

	// all the interactable tiles
	std::vector<Tile*> m_InteractableTiles;

	// collisions
	std::vector<Tile*> m_LevelWallColliders;

	// position of characters
	Character* m_PlaySpawn;
	Character* m_EnemySpawn;

	char m_LevelArray[m_LevelWidth][m_LevelHeight];

	Level(int _levelWidth, int _levelHeight);
	~Level();

	void LoadLevel(std::string _filePath);
	void SetupLevel();
	void UnloadLevel();
};

