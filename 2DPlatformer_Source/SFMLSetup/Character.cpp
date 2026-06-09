#include "Character.h"

Character::Character(CharacterType _Type)
{
	if (_Type == CType_Player)
	{
		//Shape
		m_CharShape = new sf::RectangleShape({ 48,48 });
		//m_CharShape->setFillColor(sf::Color::Cyan);

		//Texture
		m_CharTex = new sf::Texture();
		if (m_CharTex->loadFromFile("Sprites/PlayerSprite.png"))
		{
			m_CharShape->setTexture(m_CharTex);

			//Initialise first walking frame
			m_AnimationRect = sf::IntRect{ sf::Vector2i{1,13}, sf::Vector2i{48,48} };
			m_CharShape->setTextureRect(m_AnimationRect);
		}
	}
	if (_Type == CType_Enemy)
	{
		//Shape
		m_CharShape = new sf::RectangleShape({ 38,28 });
		//m_CharShape->setFillColor(sf::Color::Cyan);

		//Texture
		m_CharTex = new sf::Texture();
		if (m_CharTex->loadFromFile("Sprites/EnemySprite.png"))
		{
			m_CharShape->setTexture(m_CharTex);

			//Initialise first walking frame
			m_AnimationRect = sf::IntRect{ sf::Vector2i{124,40}, sf::Vector2i{38,28} };
			m_CharShape->setTextureRect(m_AnimationRect);
		}
	}

	//Position
	m_CharPos = sf::Vector2f(300, 300);
	m_CharShape->setPosition(m_CharPos);

	m_CharShape->setOrigin(m_CharShape->getSize() * 0.5f);
}

Character::~Character()
{
}

void Character::CharacterUpdate(float _dt, std::vector<Tile*> _Collisions)
{
	m_CharShape->move({ 0, m_PlayerYVelocity });
	for (int i = 0; i < _Collisions.size(); i++)
	{
		if (_Collisions[i]->m_TileType != tile_Barrier)
		{
			if (_Collisions[i]->m_TileType == tile_Platform && m_PassThrough)
			{
				m_PassThrough = false;
				return;
			}

			if (m_CharShape->getGlobalBounds().findIntersection(*_Collisions[i]->m_TileCollider))
			{
				YColliding = true;
				if (m_CharShape->getPosition().y < _Collisions[i]->m_TileCollider->position.y)
				{
					m_Jumping = false;
					m_CurrentState = State_Idling;
				}

				m_PlayerYVelocity = 0;
				Collisions::ResolveYCollisions(GetCharShape(), _Collisions[i]->m_TileCollider);
			}
			else
			{
				YColliding = false;
			}
		}
	}


	m_CharShape->move({m_CharMoveVec.x * m_CharSpeed * _dt, 0});
	for (int i = 0; i < _Collisions.size(); i++)
	{
		if (m_CharShape->getGlobalBounds().findIntersection(*_Collisions[i]->m_TileCollider))
		{
			//For enemy to move back when they hit a wall or movement barrier
			if (m_CharType == CType_Enemy)
			{
				if (_Collisions[i]->m_TileType == tile_Barrier || _Collisions[i]->m_TileType == tile_Ground || _Collisions[i]->m_TileType == tile_Door)
				{
					if (m_CharShape->getPosition().x < _Collisions[i]->m_TileCollider->position.x)
					{
						if (m_FacingLeft)
						{
							m_CharShape->setScale(sf::Vector2f(1, 1));
							m_FacingLeft = false;
						}
					}

					if (m_CharShape->getPosition().x > _Collisions[i]->m_TileCollider->position.x)
					{
						if (!m_FacingLeft)
						{
							m_CharShape->setScale(sf::Vector2f(-1, 1));
							m_FacingLeft = true;
						}
					}
				}
			}

			if (_Collisions[i]->m_TileType != tile_Barrier )
			{
				if (_Collisions[i]->m_TileType != tile_Door)
				{
					Collisions::ResolveXCollisions(GetCharShape(), _Collisions[i]->m_TileCollider);
				}
				else
				{
					if (!m_DoorOpen)
					{
						Collisions::ResolveXCollisions(GetCharShape(), _Collisions[i]->m_TileCollider);
					}
				}
			}
		}
	}

	m_CharPos = m_CharShape->getPosition();
}

void Character::CharacterInputUpdate(float _dt, std::vector<Tile*> _Collisions, bool _DoorKey)
{
	m_DoorOpen = _DoorKey;

	if (m_CharType == CType_Player)
	{
		m_CharMoveVec = sf::Vector2f(0.0f, 0.0f);

		if (m_CurrentState != State_Jumping)
		{
			m_CurrentState = State_Idling;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			if (!m_Jumping)
			{
				m_Jumping = true;
				m_CurrentState = State_Jumping;
				m_PlayerYVelocity = -8.5;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			m_PassThrough = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			m_CharMoveVec.x = -1;
			m_FacingLeft = true;
			if (m_CurrentState != State_Jumping)
			{
				m_CurrentState = State_Walking;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			if (m_FacingLeft)
			{
				m_FacingLeft = false;
				m_CharShape->setScale(sf::Vector2f(1, 1));
			}
			m_CharMoveVec.x = 1;
			if (m_CurrentState != State_Jumping)
			{
				m_CurrentState = State_Walking;
			}
		}

		CharacterGravUpdate(_dt);
		PlayerAnimate();
		CharacterUpdate(_dt, _Collisions);

		if (m_DoorOpen)
		{
			for (int i = 0; i < _Collisions.size(); i++)
			{
				if (_Collisions[i]->m_TileType == tile_Door)
				{
					_Collisions[i]->m_TileShape.setFillColor(sf::Color::White);
				}
			}
		}
	}
	else if (m_CharType == CType_Enemy)
	{
		m_CurrentState = State_Walking;

		if (m_FacingLeft)
		{
			m_CharMoveVec.x = 0.3;
		}
		else
		{
			m_CharMoveVec.x = -0.3;
		}

		CharacterGravUpdate(_dt);
		EnemyAnimate();
		CharacterUpdate(_dt, _Collisions);
	}
}

void Character::CharacterGravUpdate(float _dt)
{
	if (!YColliding)
	{
		m_PlayerYVelocity += m_Grav * _dt;
	}
	else
	{
		m_PlayerYVelocity = 0;
	}
}

void Character::PlayerAnimate()
{
	switch (m_CurrentState)
	{
	case State_Idling:
		//Play idle animation
		if (m_AnimationClock.getElapsedTime().asSeconds() > 0.16)
		{
			m_AnimationRect.position.x = 1;
			m_AnimationRect.position.y = 13;

			m_CharShape->setTextureRect(m_AnimationRect);
			m_AnimationClock.restart();
		}
		break;
	case State_Walking:
		//Play waling animation
		if (m_AnimationClock.getElapsedTime().asSeconds() > 0.08)
		{
			if (m_AnimationRect.position.x < 540)
			{
				m_AnimationRect.position.x += 49;
			}
			else if (m_AnimationRect.position.x >= 540)
			{
				m_AnimationRect.position.x = 1;
			}
			m_AnimationRect.position.y = 142;

			if (m_FacingLeft)
			{
				//For left facing
				m_CharShape->setScale(sf::Vector2f(-1, 1));
				m_CharShape->setTextureRect(m_AnimationRect);
			}
			else
			{
				m_CharShape->setTextureRect(m_AnimationRect);
			}
			
			m_AnimationClock.restart();
		}
		break;
	case State_Jumping:
		if (m_AnimationClock.getElapsedTime().asSeconds() > 0.08)
		{
			//sets to the jumping sprite position
			if (m_AnimationRect.position.x >= 1 && m_AnimationRect.position.x < 197)
			{
				m_AnimationRect.position.x = 197;
			}

			if (m_AnimationRect.position.x < 932)
			{
				m_AnimationRect.position.x += 49;
			}
			else if (m_AnimationRect.position.x >= 932)
			{
				m_AnimationRect.position.x = 197;
			}
			m_AnimationRect.position.y = 270;

			if (m_FacingLeft)
			{
				//For left facing
				m_CharShape->setScale(sf::Vector2f(-1, 1));
				m_CharShape->setTextureRect(m_AnimationRect);
			}
			else
			{
				m_CharShape->setTextureRect(m_AnimationRect);
			}

			m_AnimationClock.restart();
		}
		break;
	default:
		break;
	}
}

void Character::EnemyAnimate()
{
	switch (m_CurrentState)
	{
	case State_Walking:
		//Play waling animation
		if (m_AnimationClock.getElapsedTime().asSeconds() > 0.08)
		{
			if (m_AnimationRect.position.x < 204)
			{
				m_AnimationRect.position.x += 40;
			}
			else if (m_AnimationRect.position.x >= 204)
			{
				m_AnimationRect.position.x = 124;
			}
			m_AnimationRect.position.y = 40;

			m_CharShape->setTextureRect(m_AnimationRect);
			m_AnimationClock.restart();
		}
		break;
	default:
		break;
	}
}


