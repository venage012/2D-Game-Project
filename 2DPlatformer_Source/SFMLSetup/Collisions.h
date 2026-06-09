#pragma once
#include <SFML/Graphics.hpp>

static class Collisions
{
public:
	static void ResolveXCollisions(sf::Shape* _objA, sf::FloatRect* _objB)
	{
		sf::Vector2f entityACenter = _objA->getGlobalBounds().getCenter();
		sf::Vector2f entityBCenter = _objB->getCenter();

		if (entityACenter.x <= entityBCenter.x)
		{
			float offset = -(_objA->getGlobalBounds().position.x + _objA->getGlobalBounds().size.x - _objB->position.x);
			_objA->move({ offset,0 });
		}

		if (entityACenter.x >= entityBCenter.x)
		{
			float offset = (_objB->position.x + _objB->size.x) - _objA->getGlobalBounds().position.x;
			_objA->move({ offset, 0 });
		}
	}

	static void ResolveYCollisions(sf::Shape* _objA, sf::FloatRect* _objB)
	{
		sf::Vector2f entityACenter = _objA->getGlobalBounds().getCenter();
		sf::Vector2f entityBCenter = _objB->getCenter();

		if (entityACenter.y <= entityBCenter.y)
		{
			float offset = -(_objA->getGlobalBounds().position.y + _objA->getGlobalBounds().size.y - _objB->position.y);
			_objA->move({ 0, offset });
		}

		if (entityACenter.y >= entityBCenter.y)
		{
			float offset = (_objB->position.y + _objB->size.y) - _objA->getGlobalBounds().position.y;
			_objA->move({ 0, offset });
		}
	}

};