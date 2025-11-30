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
	const int fieldNumber;	// size of the board
public:
	const float breedFatThreshold = 1.f;	// fat threshold for wolf breeding
	std::vector<GameCharacter*> gameCharacterList;	// main list of characters on the board
	std::vector<GameCharacter*> deadCharacters, newCharacters;	// lists for deaths and births
	std::vector<Animation*> animationList;	// list of active animations
	Board(int size);

	// fieldNumber getter
	int getFieldNumber() const;

	// Get total character count of specific type
	int getCharacterCount(CharacterType type) const;

	// Check if position is out of bounds
	bool checkOutOfBounds(Position pos) const;

	// Add new character to the board
	void addCharacter(Position pos, CharacterType type);

	// Remove character from list
	void removeCharacter(GameCharacter* character);

	// Check if character can move at all
	bool canMove(GameCharacter* character) const;

	// Check if character can move to a position
	bool canMoveTo(Position pos, bool isWolf = false) const;

	// Generate random move
	sf::Vector2i randomMove(GameCharacter* character) const;

	// Count all characters on a field
	int countCharactersOnField(Position pos) const;

	// Count specific character type on a field
	int countCharactersOnField(Position pos, CharacterType type) const;

	// Get first character of specific type at a position
	GameCharacter* getCharacterAt(Position pos, CharacterType type) const;

	// Get first character at a position
	GameCharacter* getCharacterAt(Position pos) const;

	// Check if wolf can eat rabbit at position
	bool feastTime(Position pos) const;

	// Get closest character of specific type
	GameCharacter* getClosestCharacter(Position pos, CharacterType type) const;

	// Check if wolf breeding is possible at position
	bool wolfBreedPossible(Position pos) const;

	// Check if rabbit breeding is possible at position
	bool rabbitBreedPossible(Position pos) const;

	// Update main list and clear death/birth lists
	void applyDeathsAndBirths();

	// Update animations
	void updateAnimations();

	// Draw all active animations
	void drawAnimations(sf::RenderWindow* window, unsigned int screenSize, unsigned int fieldNumber) const;

};

