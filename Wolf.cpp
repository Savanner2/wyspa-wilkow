#include "Wolf.h"
#include <random>

Wolf::Wolf(Position pos) : GameCharacter(pos) {
	this->fat = 1.f;
	this->type = (rand() % 2 == 0) ? CharacterType::WOLF : CharacterType::WOLFESS;
}

Wolf::Wolf(Position pos, CharacterType type) : GameCharacter(pos) {
	this->pos = pos;
	this->fat = 1.f;
	this->type = type;
}

void Wolf::Move(int deltaX, int deltaY)
{
	this->pos.x += deltaX;
	this->pos.y += deltaY;

	this->fat -= 0.1;
}

void Wolf::Draw(sf::RenderWindow* window, sf::Texture& texture, unsigned int screenSize, unsigned int fieldNumber, int p)
{
	float fieldSize = (float)(screenSize / fieldNumber);
	float radius = fieldSize / 5 * (9.f / 10);
	sf::Vector2f position;
	sf::CircleShape circle(radius);
	circle.setFillColor(this->type == CharacterType::WOLF ? sf::Color::Blue : sf::Color::Magenta);

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

	sf::Sprite wolf(texture);
	wolf.setScale(sf::Vector2f({
		(fieldSize / wolf.getLocalBounds().size.x) / 3,
		(fieldSize / wolf.getLocalBounds().size.y) / 3
		}));

	wolf.setPosition(position);
	window->draw(wolf);
}

sf::Texture* Wolf::GetTexture(class Game* game) {
	return &game->wolfTexture;
}
