#pragma once
#include "Board.h"
#include <SFML/Graphics.hpp>
#include "Windows.h"

class Game
{
	unsigned int boardSize;
	sf::RectangleShape clearAllButton;
	bool spacePrevState = false;
	float lineThickness = 2.f;
	sf::Font fontGoogleSans;
	sf::Font fontEmoji;

	static std::vector<uint8_t> loadResource(WORD resourceId, const wchar_t* resourceType);
	std::vector<uint8_t> fontGoogleSansBuffer;
	std::vector<uint8_t> fontEmojiBuffer;
public:
	Board board;
	unsigned int screenSize;
	float rabbitBreedChance = 0.2f;
	float rabbitIdleChance = 1.f / 9;

	int speed = 60, tick = 0;
	float frameDelay = 1.f;
	bool paused = true;
	int characterSelected = 1; // 1 - Rabbit, 2 - Wolf, 3 - Wolfess

	sf::Texture backgroundTexture;
	sf::Texture rabbitTexture;
	sf::Texture wolfTexture;
	sf::Texture eatTexture;
	sf::Texture deathTexture;
	sf::Texture rabbitBreedTexture;
	sf::Texture wolfBreedTexture;
	sf::Texture hedgeTexture;


	Game(unsigned int boardSize, unsigned int screenSize);
	bool loadTextures();
	void drawGrid(sf::RenderWindow* window, int screenSize, int fieldNumber) const;
	void drawBackground(sf::RenderWindow* window, int screenSize, int fieldNumber) const;
	// draw controls
	void drawControls(sf::RenderWindow* window);
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

	void handleEvents(sf::RenderWindow* window, const std::optional<sf::Event>* e);

};

