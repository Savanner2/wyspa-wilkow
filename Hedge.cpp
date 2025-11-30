#include "Hedge.h"
#include <SFML/Graphics.hpp>

Hedge::Hedge(Position pos) : GameCharacter(pos)
{
	this->type = CharacterType::HEDGE;
	this->slots = 0;
}

void Hedge::Draw(sf::RenderWindow* window, sf::Texture& texture, unsigned int screenSize, unsigned int fieldNumber, int p)
{
	float fieldSize = (float)(screenSize / fieldNumber);
	sf::Vector2f position;

	position = {
		((pos.x - 1) * fieldSize),// - fieldSize + fieldSize * (1.f / 10)),
		((pos.y - 1) * fieldSize) // - (fieldSize / 2) - radius)
	};

	sf::Sprite hedge(texture);
	hedge.setScale(sf::Vector2f({
		(fieldSize / hedge.getLocalBounds().size.x) ,
		(fieldSize / hedge.getLocalBounds().size.y)
		}));

	hedge.setPosition(position);
	window->draw(hedge);
}

sf::Texture* Hedge::GetTexture(class Game* game)
{
	return &game->hedgeTexture;
}
