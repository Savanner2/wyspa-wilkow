#pragma once
#include "GameCharacter.h"
#include "Game.h"

class Rabbit : public GameCharacter
{
public:
	Rabbit(Position pos);

	// get rabbit texture
	static sf::Texture* GetTexture(class Game* game);
};

