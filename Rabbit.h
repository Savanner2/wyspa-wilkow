#pragma once
#include "GameCharacter.h"
#include "Game.h"

class Rabbit : public GameCharacter
{
public:
    double breedChance = 0.2;
	Rabbit(Position pos);
	void Move(int deltaX, int deltaY) override;
	void Draw(sf::RenderWindow* window, sf::Texture& texture, unsigned int screenSize, unsigned int fieldNumber, int p = 0);

	static sf::Texture* GetTexture(class Game* game);
};

