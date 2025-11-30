#pragma once
#include "GameCharacter.h"
#include "Game.h"

class Hedge :
    public GameCharacter
{
    public:
    Hedge(Position pos);
    void Draw(sf::RenderWindow* window, sf::Texture& texture, unsigned int screenSize, unsigned int fieldNumber, int p = 0) override;
	static sf::Texture* GetTexture(class Game* game);
};

