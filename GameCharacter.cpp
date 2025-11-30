#include "GameCharacter.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>

GameCharacter::GameCharacter(Position pos) {
	this->pos = pos;
	this->slots = 1;
}

void GameCharacter::Move(int deltaX, int deltaY) {
	this->pos.x += deltaX;
	this->pos.y += deltaY;
}

void GameCharacter::Draw(sf::RenderWindow* window, sf::Texture& texture, unsigned int screenSize, unsigned int fieldNumber, int p) {
	float fieldSize = (float)(screenSize / fieldNumber);
	float radius = fieldSize / 5 * (9.f / 10);
	sf::Vector2f position;
	sf::CircleShape circle(radius);
	switch (this->type)
	{
		case CharacterType::RABBIT:
			circle.setFillColor(sf::Color::Red);
			break;
		case CharacterType::WOLF:
			circle.setFillColor(sf::Color::Blue);
			break;
		case CharacterType::WOLFESS:
			circle.setFillColor(sf::Color::Magenta);
			break;
	}

	// p = 0 -> draw in the middle
	// p = 1 -> draw on the left
	// p = 2 -> draw on the right
	// p = 3 -> draw on the top


	switch (p) {
	case 1:
		position = {
			(pos.x * fieldSize - fieldSize + fieldSize * (1.f / 10)),
			(pos.y * fieldSize - (fieldSize / 2) - radius)
		};
		break;
	case 2:
		position = {
			(pos.x * fieldSize - radius * 2 - fieldSize * (1.f / 10)),
			(pos.y * fieldSize - (fieldSize / 2) - radius)
		};
		break;
	case 3:
		position = {
			(pos.x * fieldSize - (fieldSize / 2) - radius),
			(pos.y * fieldSize - fieldSize + fieldSize * (1.f / 10))
		};
		break;
	default:
		position = {
			(pos.x * fieldSize - (fieldSize / 2) - radius),
			(pos.y * fieldSize - (fieldSize / 2) - radius)
		};
		break;
	}

	circle.setPosition(position);
	window->draw(circle);

	sf::Sprite sprite(texture);
	sprite.setScale(sf::Vector2f({
		(fieldSize / sprite.getLocalBounds().size.x) / 2.8f,
		(fieldSize / sprite.getLocalBounds().size.y) / 2.8f
		}));

	sprite.setPosition(position);
	window->draw(sprite);
}
