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
	
	if (_Type == CType_Enemy)
	{
		//Shape
		m_CharShape = new sf::RectangleShape({ 40,40 });
		m_CharShape->setFillColor(sf::Color::Red);
	}
	

	//Position
	m_CharPos = sf::Vector2f(300, 300);
	m_CharShape->setPosition(m_CharPos);

	m_CharShape->setOrigin(m_CharShape->getSize() * 0.5f);
}

Character::~Character()
{
}

void Character::CharacterUpdate(float _dt, std::vector<Tile*> _Collisions, sf::RectangleShape _Bullet, int& _BoxCount)
{
	m_CharShape->move({ 0, m_CharMoveVec.y * m_CharSpeed * _dt });
	for (int i = 0; i < _Collisions.size(); i++)
	{
		//Box code if the bullet hits it
		if (_Collisions[i]->m_TileType == tile_BreakBox)
		{
			if (_Bullet.getGlobalBounds().findIntersection(*_Collisions[i]->m_TileCollider))
			{
				if (_Collisions[i]->BoxBroken != true)
				{
					_Collisions[i]->m_TileShape.setFillColor(sf::Color::White);
					_BoxCount--;
					_Collisions[i]->BoxBroken = true;
					std::cout << _BoxCount << " boxes remain" << std::endl;
					if (_BoxCount == 0)
					{
						std::cout << "Target is now vulnerable, finish the job" << std::endl;
					}
				}
			}
		}

		if (m_CharShape->getGlobalBounds().findIntersection(*_Collisions[i]->m_TileCollider))
		{
			YColliding = true;
			if (m_CharShape->getPosition().y < _Collisions[i]->m_TileCollider->position.y)
			{
				m_Jumping = false;
			}

			m_PlayerYVelocity = 0;

			//For box collisions
			if (_Collisions[i]->m_TileType != tile_BreakBox)
			{
				Collisions::ResolveYCollisions(GetCharShape(), _Collisions[i]->m_TileCollider);
			}
			else
			{
				if (_Collisions[i]->BoxBroken == false)
				{
					Collisions::ResolveYCollisions(GetCharShape(), _Collisions[i]->m_TileCollider);
				}
			}
		}
		else
		{
			YColliding = false;
		}

	}


	m_CharShape->move({m_CharMoveVec.x * m_CharSpeed * _dt, 0});
	for (int i = 0; i < _Collisions.size(); i++)
	{
		if (m_CharShape->getGlobalBounds().findIntersection(*_Collisions[i]->m_TileCollider))
		{
			if (_Collisions[i]->m_TileType != tile_BreakBox)
			{
				Collisions::ResolveXCollisions(GetCharShape(), _Collisions[i]->m_TileCollider);
			}
			else
			{
				if (_Collisions[i]->BoxBroken == false)
				{
					Collisions::ResolveXCollisions(GetCharShape(), _Collisions[i]->m_TileCollider);
				}
			}

		}
	}

	m_CharPos = m_CharShape->getPosition();
}

void Character::CharacterInputUpdate(float _dt, std::vector<Tile*> _Collisions, sf::Vector2f _Mouse, sf::RectangleShape _Bullet, int& _BoxCount)
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

		PlayerAnimate();
		CharacterUpdate(_dt, _Collisions, _Bullet, _BoxCount);
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
			m_CharShape->setTextureRect(m_AnimationRect);
			m_AnimationClock.restart();
		}
		break;
	default:
		break;
	}
}



