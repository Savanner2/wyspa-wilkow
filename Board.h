#pragma once
#include "GameCharacter.h"
#include<vector>
#include "Position.h"
#include "CharacterType.h"
#include <SFML/System/Vector2.hpp>
#include "Animation.h"

class Board
{
	const int MAX_CHARACTERS_PER_FIELD = 3;
	const int fieldNumber;
public:
	const float breedFatThreshold = 1.f;
	std::vector<GameCharacter*> gameCharacterList;
	std::vector<GameCharacter*> deadCharacters, newCharacters;
	std::vector<Animation*> animationList;
	Board(int size);

	// fieldNumber getter
	int getFieldNumber() const;

	// get total character count of specific type
	int getCharacterCount(CharacterType type) const;

	// check if position is out of bounds
	bool checkOutOfBounds(Position pos) const;

	// add character to the board
	void addCharacter(Position pos, CharacterType type);

	// remove character from list
	void removeCharacter(GameCharacter* character);

	// check if character can move at all
	bool canMove(GameCharacter* character) const;

	// check if character can move to a position
	bool canMoveTo(Position pos, bool isWolf = false) const;

	// generate random move
	sf::Vector2i randomMove(GameCharacter* character) const;

	// count all character types on a field
	int countCharactersOnField(Position pos) const;

	// count specific character type on a field
	int countCharactersOnField(Position pos, CharacterType type) const;

	// get character of specific type at a position
	GameCharacter* getCharacterAt(Position pos, CharacterType type) const;

	// get character at a position
	GameCharacter* getCharacterAt(Position pos) const;

	// check if wolf can eat rabbit at position
	bool feastTime(Position pos) const;

	// get nearby character of specific type
	GameCharacter* getClosestCharacter(Position pos, CharacterType type) const;

	// check if wolf breeding is possible at position
	bool wolfBreedPossible(Position pos) const;

	// check if rabbit breeding is possible at position
	bool rabbitBreedPossible(Position pos) const;

	// apply death and births
	void applyDeathsAndBirths();

	// update animations
	void updateAnimations();

	// draw animations
	void drawAnimations(sf::RenderWindow* window, unsigned int screenSize, unsigned int fieldNumber) const;

	
};

