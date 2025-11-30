#pragma once  

#include "GameCharacter.h"  
#include <SFML/Graphics.hpp>
#include "Game.h"

class Wolf : public GameCharacter  
{  
public:  
    double fat;
	Wolf(Position pos);
	Wolf(Position pos, CharacterType type);
    void Move(int deltaX, int deltaY) override;

    static sf::Texture* GetTexture(class Game* game);
};
