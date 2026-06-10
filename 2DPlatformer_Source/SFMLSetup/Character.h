#pragma once
#include "CHelper.h"
#include "Tile.h"

enum CharacterState
{
	State_Idling,
	State_Walking,
	State_Jumping
};

enum CharacterType
{
	CType_Player,
	CType_Enemy
};

class Character
{
public:
	sf::RectangleShape* m_CharShape;
	sf::Texture* m_CharTex;
	
	sf::Vector2f m_CharPos; //Shape is bound to this position. (Character)
	sf::Vector2f m_CharMoveVec; // Holds the current movement vector.

	float m_CharSpeed = 300.0f;
	float m_CharBaseSpeed = 30.0f;

	//Physics stuff
	bool YColliding = false;
	float m_PlayerYVelocity = 0.0f; //No x velocity because not making x axis momentum based movement
	float m_Grav = 10.0f;

	//Movement stuff
	CharacterState m_CurrentState;
	bool m_FacingLeft = false;
	bool m_Jumping = false;
	bool m_PassThrough = false;
	bool m_DoorOpen = false;
	sf::Clock m_AnimationClock;
	sf::IntRect m_AnimationRect;

	CharacterType m_CharType;

	//Functions
	Character(CharacterType _Type);
	~Character();

	void CharacterUpdate(float _dt, std::vector<Tile*> _Collisions);
	void CharacterInputUpdate(float _dt, std::vector<Tile*> _Collisions, sf::Vector2f _Mouse);
	void CharacterGravUpdate(float _dt);
	void PlayerAnimate();
	void EnemyAnimate();

	sf::RectangleShape* GetCharShape() { return m_CharShape; }
};

