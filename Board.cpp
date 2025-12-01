#include "Board.h"
#include "GameCharacter.h"
#include "CharacterType.h"
#include "Animation.h"
#include "Wolf.h"
#include "Rabbit.h"
#include "Hedge.h"

#include <vector>
#include <iostream>
#include <cstdlib>

Board::Board(int size)
	: fieldNumber(size)
{
}

int Board::getFieldNumber() const
{
	return fieldNumber;
}

int Board::getCharacterCount(CharacterType type) const
{
	int count = 0;
	for (const GameCharacter* character : gameCharacterList) {
		if (character->type == type)
			count++;
	}
	return count;
}

bool Board::checkOutOfBounds(Position pos) const
{
	return pos.x < 1 || pos.x > fieldNumber || pos.y < 1 || pos.y > fieldNumber;
}

void Board::addCharacter(Position pos, CharacterType type) {
	switch (type) {
		case CharacterType::RABBIT:
			if(canMoveTo(pos))
				gameCharacterList.push_back(new Rabbit(pos));
			break;
		case CharacterType::WOLF:
			if (getCharacterAt(pos, CharacterType::HEDGE) == nullptr && canMoveTo(pos, true))
				gameCharacterList.push_back(new Wolf(pos, CharacterType::WOLF));
			break;
		case CharacterType::WOLFESS:
			if (getCharacterAt(pos, CharacterType::HEDGE) == nullptr && canMoveTo(pos, true))
				gameCharacterList.push_back(new Wolf(pos, CharacterType::WOLFESS));
			break;
		case CharacterType::HEDGE:
			if (canMoveTo(pos) &&
				getCharacterAt(pos, CharacterType::WOLF) == nullptr &&
				getCharacterAt(pos, CharacterType::WOLFESS) == nullptr &&
				getCharacterAt(pos, CharacterType::HEDGE) == nullptr
			) {
				gameCharacterList.push_back(new Hedge(pos));
			}
			break;
	}
}

void Board::removeCharacter(GameCharacter* character)
{
	if (!character) return;
	gameCharacterList.erase(
		std::remove(gameCharacterList.begin(), gameCharacterList.end(), character),
		gameCharacterList.end()
	);
	delete character;
}

bool Board::canMove(GameCharacter* character) const
{
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			if (canMoveTo(Position(character->pos.x + i, character->pos.y + j), character->type == CharacterType::WOLF || character->type == CharacterType::WOLFESS))
				return true;
			
		}
	}
	return false;
}

bool Board::canMoveTo(Position pos, bool isWolf) const
{
	if(!isWolf)
		return !checkOutOfBounds(pos) && countCharactersOnField(pos) < MAX_CHARACTERS_PER_FIELD;
	else
		return !checkOutOfBounds(pos) && countCharactersOnField(pos) < MAX_CHARACTERS_PER_FIELD && getCharacterAt(pos, CharacterType::HEDGE) == nullptr;
}

sf::Vector2i Board::randomMove(GameCharacter* character) const
{
	int attempts = 0;
	int deltaX = (rand() % 3) - 1;
	int deltaY = (rand() % 3) - 1;
	bool isWolf = (character->type == CharacterType::WOLF || character->type == CharacterType::WOLFESS);
	while (!canMoveTo(Position(character->pos.x + deltaX, character->pos.y + deltaY), isWolf) ||
		(deltaX == 0 && deltaY == 0)
		)
	{
		attempts++;
		if(attempts > 100) 
			return sf::Vector2i{ 0, 0 };
		deltaX = (rand() % 3) - 1;
		deltaY = (rand() % 3) - 1;
	}
	return sf::Vector2i{ deltaX, deltaY };
}

int Board::countCharactersOnField(Position pos) const
{
	int count = 0;
	for (const GameCharacter* character : gameCharacterList) {
		if (character->pos.x == pos.x && character->pos.y == pos.y) {
			count += character->slots;
		}
	}
	for (const GameCharacter* character : newCharacters) {
		if (character->pos.x == pos.x && character->pos.y == pos.y) {
			count += character->slots;
		}
	}
	return count;
}

int Board::countCharactersOnField(Position pos, CharacterType type) const
{
	int count = 0;
	for (const GameCharacter* character : gameCharacterList) {
		if (character->pos.x == pos.x && character->pos.y == pos.y && character->type == type)
			count += character->slots;
	}
	for (const GameCharacter* character : newCharacters) {
		if (character->pos.x == pos.x && character->pos.y == pos.y && character->type == type)
			count += character->slots;
	}
	return count;
}

GameCharacter* Board::getCharacterAt(Position pos, CharacterType type) const
{
	for (GameCharacter* character : gameCharacterList) {
		if (character->pos.x == pos.x && character->pos.y == pos.y)
			if (character->type == type)
				return character;
	}

	return nullptr;
}

GameCharacter* Board::getCharacterAt(Position pos) const
{
	for (GameCharacter* character : gameCharacterList) {
		if (character->pos.x == pos.x && character->pos.y == pos.y)
				return character;
	}

	return nullptr;
}

bool Board::feastTime(Position pos) const
{
	return (Board::getCharacterAt(pos, CharacterType::RABBIT) != nullptr && Board::getCharacterAt(pos, CharacterType::WOLF) != nullptr) ||
		(Board::getCharacterAt(pos, CharacterType::RABBIT) != nullptr && Board::getCharacterAt(pos, CharacterType::WOLFESS) != nullptr);
}

GameCharacter* Board::getClosestCharacter(Position pos, CharacterType type) const
{
	GameCharacter* closest = nullptr;
	int minDistance = fieldNumber;
	for (GameCharacter* character : gameCharacterList) {
		if (character->type == type) {
			if (std::max(std::abs(character->pos.x - pos.x), std::abs(character->pos.y - pos.y)) < minDistance) {
				minDistance = std::max(std::abs(character->pos.x - pos.x), std::abs(character->pos.y - pos.y));
				closest = character;
			}
			
		}
	}
	for (GameCharacter* character : newCharacters) {
		if (character->type == type) {
			if (std::max(std::abs(character->pos.x - pos.x), std::abs(character->pos.y - pos.y)) < minDistance) {
				minDistance = std::max(std::abs(character->pos.x - pos.x), std::abs(character->pos.y - pos.y));
				closest = character;
			}

		}
	}
	return closest;
}

bool Board::wolfBreedPossible(Position pos) const
{
	if (countCharactersOnField(pos) < MAX_CHARACTERS_PER_FIELD) {
		Wolf* wolf = dynamic_cast<Wolf*>(getCharacterAt(pos, CharacterType::WOLF));
		Wolf* wolfess = dynamic_cast<Wolf*>(getCharacterAt(pos, CharacterType::WOLFESS));

		if (wolf != nullptr && wolfess != nullptr) {
			return wolf->fat >= breedFatThreshold;
		}
	}
	return false;
}

bool Board::rabbitBreedPossible(Position pos) const
{
	return countCharactersOnField(pos) < MAX_CHARACTERS_PER_FIELD;
	
}

void Board::applyDeathsAndBirths()
{
	for (GameCharacter* dead : deadCharacters) {
		removeCharacter(dead);
	}

	for (GameCharacter* born : newCharacters) {
		addCharacter(born->pos, born->type);
	}

	deadCharacters.clear();
	newCharacters.clear();
}

void Board::updateAnimations()
{
	for(auto it = animationList.begin(); it != animationList.end(); ) {
		Animation* animation = *it;
		if (animation) {
			animation->update();
			if (animation->isFinished()) {
				it = animationList.erase(it);
				delete animation;
			} else {
				++it;
			}
		} else {
			it = animationList.erase(it);
		}
	}
}

void Board::drawAnimations(sf::RenderWindow* window, unsigned int screenSize, unsigned int fieldNumber) const
{
	for (Animation* animation : animationList) {
		animation->draw(window, screenSize, fieldNumber);
	}
}


