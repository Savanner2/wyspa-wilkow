#pragma once
#include "Board.h"
#include <SFML/Graphics.hpp>
#include "Windows.h"

class Game
{
	const unsigned int boardSize;	// size of the board
	const unsigned int screenSize;	// size of the board in pixels
	float lineThickness = 2.f;	// thickness of grid lines
	sf::Font fontMain;	// main font
	sf::Font fontEmoji;			// emoji font
	std::vector<uint8_t> fontMainBuffer;	// buffer for loading main font from resources
	std::vector<uint8_t> fontEmojiBuffer;	// buffer for loading emoji font from resources
	sf::RectangleShape clearAllButton; // clear all button

	// load resource
	static std::vector<uint8_t> loadResource(WORD resourceId, const wchar_t* resourceType);
public:
	Board board;	// main game board
	const float rabbitBreedChance = 0.2f;		// 20% chance of rabbit breeding
	const float rabbitIdleChance = 1.f / 9;		// 1/9 chance of rabbit idling

	int speed = 60;	// ticks per second
	int tick = 0;	// current tick
	float frameDelay = 1.f;	// frame delay ratio
	bool paused = true;	// is the game paused
	int characterSelected = 1; // 1 - Rabbit, 2 - Wolf, 3 - Wolfess, 0 - Hedge

	sf::Texture backgroundTexture;
	sf::Texture rabbitTexture;
	sf::Texture wolfTexture;
	sf::Texture eatTexture;
	sf::Texture deathTexture;
	sf::Texture rabbitBreedTexture;
	sf::Texture wolfBreedTexture;
	sf::Texture hedgeTexture;

	// constructor
	Game(unsigned int boardSize, unsigned int screenSize);
	// load textures from resources
	bool loadTextures();
	// draw grid
	void drawGrid(sf::RenderWindow* window) const;
	// draw background
	void drawBackground(sf::RenderWindow* window) const;
	// draw controls and info
	void drawControls(sf::RenderWindow* window);
	// draw characters
	void drawCharacters(sf::RenderWindow* window) const;


	// create and return speed text
	sf::Text speedText() const;
	// create and return speed text
	sf::Text statusText() const;
	// create and return board edit text
	sf::Text boardEditText() const;
	// create pause text
	sf::Text pauseText() const;

	// get field from mouse position
	Position getFieldFromMousePosition(sf::Vector2i mousePos) const;
	// handle input events
	void handleEvents(sf::RenderWindow* window, const std::optional<sf::Event>* e);
};

