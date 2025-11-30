#pragma once
#include "Position.h"
#include <SFML/Graphics.hpp>
#include "CharacterType.h"

class GameCharacter
{
public:
	Position pos;
	CharacterType type;
	int slots;
	GameCharacter(Position pos);
	virtual void Move(int deltaX, int deltaY);
	virtual void Draw(sf::RenderWindow* window, sf::Texture& texture, unsigned int screenSize, unsigned int fieldNumber, int p = 0);
};

