#include "Level.h"

Level::Level(int _levelWidth, int _levelHeight)
{
	for (int y = 0; y < _levelHeight; y++)
	{
		for (int x = 0; x < _levelWidth; x++)
		{
			m_LevelArray[y][x] = '0';
		}
	}
	m_PlaySpawn = new Character(CType_Player);
	m_EnemySpawn = new Character(CType_Enemy);
}

Level::~Level()
{
}

void Level::LoadLevel(std::string _filePath)
{
	UnloadLevel();

	// holds data from the file. 
	std::fstream loadFileStream;
	loadFileStream.open(_filePath, std::ios::in);

	// the file is read line by line - this holds the
	// current line. 
	std::string loadFileString;
	int lineCount = 0;

	if (loadFileStream.is_open())
	{
		while (std::getline(loadFileStream, loadFileString))
		{
			for (int i = 0; i < loadFileString.size(); i++)
			{
				m_LevelArray[lineCount][i] = loadFileString[i];
			}
			lineCount++;
		}

		loadFileStream.close();
	}

	SetupLevel();
}

void Level::SetupLevel()
{
	
	for (int y = 0; y < m_LevelHeight; y++)
	{
		for (int x = 0; x < m_LevelWidth; x++)
		{
			// Walls/Floors (that you collide with)
			if (m_LevelArray[y][x] == 'X')
			{
				Tile* NewTile = new Tile(TileType::tile_Ground);
				NewTile->m_TileShape.setPosition({ x * 64.0f, y * 64.0f });

				// collision setup. 
				NewTile->m_TileCollider = new sf::FloatRect();

				// pos of the collider
				NewTile->m_TileCollider->position.x = NewTile->m_TileShape.getGlobalBounds().position.x;
				NewTile->m_TileCollider->position.y = NewTile->m_TileShape.getGlobalBounds().position.y;

				// size of the collider
				NewTile->m_TileCollider->size.x = NewTile->m_TileShape.getGlobalBounds().size.x;
				NewTile->m_TileCollider->size.y = NewTile->m_TileShape.getGlobalBounds().size.y;

				m_LevelWallTiles.push_back(NewTile);
				m_LevelWallColliders.push_back(NewTile);
			}

			// Walls (that you dont collide with)
			if (m_LevelArray[y][x] == 'O')
			{
				Tile* NewTile = new Tile(TileType::tile_Wall);
				NewTile->m_TileShape.setPosition({ x * 64.0f, y * 64.0f });

				m_LevelTiles.push_back(NewTile);
			}

			// Exit
			if (m_LevelArray[y][x] == 'E')
			{
				Tile* NewTile = new Tile(TileType::tile_Exit);
				NewTile->m_TileShape.setPosition({ x * 64.0f, y * 64.0f });

				m_LevelTiles.push_back(NewTile);
				m_InteractableTiles.push_back(NewTile);
			}

			// Player
			if (m_LevelArray[y][x] == 'P')
			{
				Tile* NewTile = new Tile(TileType::tile_Wall);
				NewTile->m_TileShape.setPosition({ x * 64.0f, y * 64.0f });

				m_LevelTiles.push_back(NewTile);
				m_PlaySpawn->m_CharShape->setPosition(NewTile->m_TileShape.getPosition());
				m_PlaySpawn->m_CharType = CType_Player;
			}

			// Enemy
			if (m_LevelArray[y][x] == 'K')
			{
				Tile* NewTile = new Tile(TileType::tile_Wall);
				NewTile->m_TileShape.setPosition({ x * 64.0f, y * 64.0f });

				m_LevelTiles.push_back(NewTile);
				m_EnemySpawn->m_CharShape->setPosition(NewTile->m_TileShape.getPosition());
				m_EnemySpawn->m_CharType = CType_Enemy;
			}

			// Breakable boxes
			if (m_LevelArray[y][x] == 'B')
			{
				Tile* NewTile = new Tile(TileType::tile_BreakBox);
				NewTile->m_TileShape.setPosition({ x * 64.0f, y * 64.0f });

				// collision setup. 
				NewTile->m_TileCollider = new sf::FloatRect();

				// pos of the collider
				NewTile->m_TileCollider->position.x = NewTile->m_TileShape.getGlobalBounds().position.x;
				NewTile->m_TileCollider->position.y = NewTile->m_TileShape.getGlobalBounds().position.y;

				// size of the collider
				NewTile->m_TileCollider->size.x = NewTile->m_TileShape.getGlobalBounds().size.x;
				NewTile->m_TileCollider->size.y = NewTile->m_TileShape.getGlobalBounds().size.y;

				m_LevelWallTiles.push_back(NewTile);
				m_LevelWallColliders.push_back(NewTile);
			}

			// Door
			if (m_LevelArray[y][x] == 'D')
			{
				Tile* NewTile = new Tile(TileType::tile_Door);
				NewTile->m_TileShape.setPosition({ x * 64.0f, y * 64.0f });

				// collision setup. 
				NewTile->m_TileCollider = new sf::FloatRect();

				// pos of the collider
				NewTile->m_TileCollider->position.x = NewTile->m_TileShape.getGlobalBounds().position.x;
				NewTile->m_TileCollider->position.y = NewTile->m_TileShape.getGlobalBounds().position.y;

				// size of the collider
				NewTile->m_TileCollider->size.x = NewTile->m_TileShape.getGlobalBounds().size.x;
				NewTile->m_TileCollider->size.y = NewTile->m_TileShape.getGlobalBounds().size.y;

				m_LevelWallTiles.push_back(NewTile);
				m_LevelWallColliders.push_back(NewTile);
			}
		}
	}
}

void Level::UnloadLevel()
{
	//Loops to delete the level tiles
	for (int l = 0; l < m_LevelTiles.size(); l++)
	{
		if(m_LevelTiles[l])
		{
			delete m_LevelTiles[l];
		}
	}

	for (int w = 0; w < m_LevelWallTiles.size(); w++)
	{
		if (m_LevelWallTiles[w])
		{
			delete m_LevelWallTiles[w];
		}
	}

	//Clears he arrays
	m_LevelTiles.clear();
	m_LevelWallTiles.clear();
	m_InteractableTiles.clear();
	m_LevelWallColliders.clear();

	//Replaces level array with '0'
	for (int y = 0; y < m_LevelHeight; y++)
	{
		for (int x = 0; x < m_LevelWidth; x++)
		{
			m_LevelArray[y][x] = '0';
		}
	}
}
