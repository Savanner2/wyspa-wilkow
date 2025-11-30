#include "GameCharacter.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>

GameCharacter::GameCharacter(Position pos) {
	this->pos = pos;
	this->slots = 1;
}

void GameCharacter::Move(int deltaX, int deltaY) {}

void GameCharacter::Draw(sf::RenderWindow* window, sf::Texture& texture, unsigned int screenSize, unsigned int fieldNumber, int p) {}
