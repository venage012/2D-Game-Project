#include "Character.h"

Character::Character(CharacterType _Type)
{
	if (_Type == CType_Player)
	{
		//Shape
		m_CharShape = new sf::RectangleShape({ 32,32 });

		//Texture
		m_CharTex = new sf::Texture();
		
		if (m_CharTex->loadFromFile("Sprites/PlayerSprite.png"))
		{
			m_CharShape->setTexture(m_CharTex);

			//Initialise first walking frame
			m_AnimationRect = sf::IntRect{ sf::Vector2i{0,0}, sf::Vector2i{16,16} };
			m_CharShape->setTextureRect(m_AnimationRect);
		}
	}
	/*
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
	*/

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
	m_CharShape->move({ 0, m_CharMoveVec.y * m_CharSpeed * _dt });
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
					//m_CurrentState = State_Idling;
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

void Character::CharacterInputUpdate(float _dt, std::vector<Tile*> _Collisions, sf::Vector2f _Mouse)
{

	if (m_CharType == CType_Player)
	{
		m_CharMoveVec = sf::Vector2f(0.0f, 0.0f);
		m_CurrentState = State_Idling;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			m_CharMoveVec.y = -1;
			m_CurrentState = State_Walking;

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			m_CharMoveVec.y = 1;
			m_CurrentState = State_Walking;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			m_CharMoveVec.x = -1;
			m_CurrentState = State_Walking;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			m_CharMoveVec.x = 1;
			m_CurrentState = State_Walking;
		}

		
		sf::Vector2f Offset = _Mouse - m_CharShape->getPosition();
		sf::Angle Rotation = sf::radians(atan2(Offset.y, Offset.x));

		m_CharShape->setRotation(Rotation);

		//CharacterGravUpdate(_dt);
		PlayerAnimate();
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
			m_AnimationRect.position.x = 0;
			m_AnimationRect.position.y = 0;

			m_CharShape->setTextureRect(m_AnimationRect);
			m_AnimationClock.restart();
		}
		break;
	case State_Walking:
		//Play waling animation
		if (m_AnimationClock.getElapsedTime().asSeconds() > 0.08)
		{
			if (m_AnimationRect.position.x < 64)
			{
				m_AnimationRect.position.x += 16;
			}
			else if (m_AnimationRect.position.x >= 64)
			{
				m_AnimationRect.position.x = 0;
			}
			m_AnimationRect.position.y = 0;

			/*
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
			*/
			m_CharShape->setTextureRect(m_AnimationRect);
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


