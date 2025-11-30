#include "Rabbit.h"
#include <SFML/Graphics.hpp>
#include <iostream>
\

Rabbit::Rabbit(Position pos) : GameCharacter(pos) {
	this->type = CharacterType::RABBIT;
}

void Rabbit::Move(int deltaX, int deltaY)
{
	this->pos.x += deltaX;
	this->pos.y += deltaY;
}

void Rabbit::Draw(sf::RenderWindow* window, sf::Texture& texture, unsigned int screenSize, unsigned int fieldNumber, int p)
{
	float fieldSize = (float)(screenSize / fieldNumber);
	float radius = fieldSize / 5 * (9.f / 10);
	sf::Vector2f position;
	sf::CircleShape circle(radius);
	circle.setFillColor(sf::Color::Red);

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

	sf::Sprite rabbit(texture);
	rabbit.setScale(sf::Vector2f({
		(fieldSize / rabbit.getLocalBounds().size.x) / 2.8f,
		(fieldSize / rabbit.getLocalBounds().size.y) / 2.8f
		}));

	rabbit.setPosition(position);
	window->draw(rabbit);
}

sf::Texture* Rabbit::GetTexture(class Game* game) {
	return &game->rabbitTexture;
}