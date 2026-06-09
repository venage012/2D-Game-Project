#pragma once
#include "Collisions.h"

class CHelper
{
public:
	sf::Clock GlobalClock;
	float m_DeltaTime = 0.0f;

	CHelper() {};
	~CHelper() {};

	void HelperTick()
	{
		m_DeltaTime = GlobalClock.getElapsedTime().asSeconds();
		GlobalClock.restart();
	}
};