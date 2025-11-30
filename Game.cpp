#include "Game.h"
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "resource1.h"
#include "Rabbit.h"
#include "Wolf.h"
#include "Hedge.h"

#define IDB_BG 101
#define IDB_BLOOD 102
#define IDB_GHEART 103
#define IDB_HEDGE 104
#define IDB_RABBIT 105
#define IDB_RHEART 106
#define IDB_SKULL 107
#define IDB_WOLF 108

Game::Game(unsigned int boardSize, unsigned int screenSize) : board(boardSize) {
	this->screenSize = screenSize;
	this->boardSize = boardSize;

	fontGoogleSansBuffer = loadResource(IDR_RCDATA1, MAKEINTRESOURCEW(RT_RCDATA));
    if (fontGoogleSansBuffer.empty()) {
        std::cerr << "Font resource for GoogleSansCode not found.\n";
    } else if (!fontGoogleSans.openFromMemory(fontGoogleSansBuffer.data(), fontGoogleSansBuffer.size())) {
        std::cerr << "Failed to open GoogleSansCode from memory.\n";
    }

    fontEmojiBuffer = loadResource(IDR_RCDATA2, MAKEINTRESOURCEW(RT_RCDATA));
    if (fontEmojiBuffer.empty()) {
        std::cerr << "Font resource for NotoSans not found.\n";
    } else if (!fontEmoji.openFromMemory(fontEmojiBuffer.data(), fontEmojiBuffer.size())) {
        std::cerr << "Failed to open NotoSans from memory.\n";
    }

    if (!loadTextures()) {
        std::cerr << "Failed to load one or more textures!" << std::endl;
    }
};


void Game::drawGrid(sf::RenderWindow* window, int screenSize, int fieldNumber) const {
    float thickness = lineThickness;
    float cellSize = (screenSize - (thickness * (fieldNumber - 1))) / fieldNumber;
    sf::Color lineColor = sf::Color::White;

    for (int i = 0; i <= fieldNumber; i++) {
        float lineStart = i == 0 ? 0 : i * cellSize + (i - 1) * thickness;
        for (float j = 0.f; j < thickness; j++) {
            // draw horizontal lines
            sf::VertexArray linesX(sf::PrimitiveType::Lines, 2);
            linesX[0].position = sf::Vector2f(0, lineStart + j);
            linesX[1].position = sf::Vector2f((float)screenSize, lineStart + j);

            linesX[0].color = lineColor;
            linesX[1].color = lineColor;
            window->draw(linesX);

            // draw vertical lines
            sf::VertexArray linesY(sf::PrimitiveType::Lines, 2);
            linesY[0].position = sf::Vector2f(lineStart + j, 0);
            linesY[1].position = sf::Vector2f(lineStart + j, (float)screenSize);

            linesY[0].color = lineColor;
            linesY[1].color = lineColor;
            window->draw(linesY);
        }
    }
}

void Game::drawBackground(sf::RenderWindow* window, int screenSize, int fieldNumber) const {
    sf::Sprite sprite(backgroundTexture);
    sf::Vector2f targetSize((float)screenSize, (float)screenSize);

    sprite.setScale(sf::Vector2f({
        targetSize.x / sprite.getLocalBounds().size.x,
        targetSize.y / sprite.getLocalBounds().size.y
        }));

    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { (int)(screenSize * (1 / sprite.getScale().x)), (int)(screenSize * (1 / sprite.getScale().y)) }));
    window->draw(sprite);
}

void Game::drawControls(sf::RenderWindow* window)
{
    if (paused) {
        window->draw(pauseText());
	}
	window->draw(speedText());
    window->draw(statusText());
	window->draw(boardEditText());


    // clear all button
    clearAllButton.setSize({ 200.f, 50.f });
	clearAllButton.setFillColor(sf::Color(200, 200, 200, 200));
    clearAllButton.setPosition({ (float)screenSize + 100, (float)(650) });
	window->draw(clearAllButton);

	sf::Text clearAllText(fontGoogleSans);
	clearAllText.setString("Clear All");
	clearAllText.setCharacterSize(28);
	clearAllText.setFillColor(sf::Color::Black);
	clearAllText.setPosition({ (float)screenSize + 125, (float)(650 + 5) });
	window->draw(clearAllText);

}

void Game::drawCharacters(sf::RenderWindow* window) const
{
    for (int i = 1; i <= boardSize; i++)
    {
        for (int j = 1; j <= boardSize; j++)
        {
            int cCount = board.countCharactersOnField(Position(i, j));
            if (cCount != 0) {
                Hedge* hedge = dynamic_cast<Hedge*>(board.getCharacterAt(Position(i, j), CharacterType::HEDGE));
                if (hedge != nullptr) {
                    hedge->Draw(window, *(Hedge::GetTexture(const_cast<Game*>(this))), screenSize, boardSize);
                }
                if (cCount > 1) {
                    int counter = 1;
                    for (GameCharacter* character : board.gameCharacterList) {
                        if (character->pos.x == i && character->pos.y == j && character->type != CharacterType::HEDGE) {
                            sf::Texture* texture = nullptr;
                            switch (character->type) {
                            case CharacterType::WOLF:
                            case CharacterType::WOLFESS:
                                texture = Wolf::GetTexture(const_cast<Game*>(this));
                                break;
                            case CharacterType::RABBIT:
                                texture = Rabbit::GetTexture(const_cast<Game*>(this));
                                break;
                            }
                            character->Draw(window, *texture, screenSize, boardSize, counter);
                            counter += character->slots;
                        }
                    }
                }
                else {
                    for (GameCharacter* character : board.gameCharacterList) {
                        if (character->pos.x == i && character->pos.y == j && character->type != CharacterType::HEDGE) {
                            sf::Texture* texture = nullptr;
                            switch (character->type) {
                            case CharacterType::WOLF:
                            case CharacterType::WOLFESS:
                                texture = Wolf::GetTexture(const_cast<Game*>(this));
                                break;
                            case CharacterType::RABBIT:
                                texture = Rabbit::GetTexture(const_cast<Game*>(this));
                                break;
                            }
                            character->Draw(window, *texture, screenSize, boardSize);
                        }
                    }
                }
            }
            else {
                Hedge* hedge = dynamic_cast<Hedge*>(board.getCharacterAt(Position(i, j), CharacterType::HEDGE));
                if (hedge != nullptr) {
                    hedge->Draw(window, *(Hedge::GetTexture(const_cast<Game*>(this))), screenSize, boardSize);
                }
            }

        }
    }
}

sf::Text Game::speedText() const
{
    sf::Text speedText(fontGoogleSans);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << (60.f / speed);

    speedText.setString("Speed: " + ss.str() + " ticks/sec\n\nPress LEFT/RIGHT to change speed\nPress SPACE to pause/resume\n\n");
	speedText.setCharacterSize(20);
	speedText.setFillColor(sf::Color::Red);
	speedText.setPosition({ (float)screenSize + 10, 10.f });

    return speedText;
}

sf::Text Game::statusText() const
{
    sf::Text statusText(fontGoogleSans);

    statusText.setString("Characters:\nRABBIT: " + std::to_string(board.getCharacterCount(CharacterType::RABBIT)) + "\n" +
		"WOLF: " + std::to_string(board.getCharacterCount(CharacterType::WOLF)) + "\n" +
		"WOLFESS: " + std::to_string(board.getCharacterCount(CharacterType::WOLFESS)) + "\n" +
		"HEDGE: " + std::to_string(board.getCharacterCount(CharacterType::HEDGE)) + "\n"
            );
    statusText.setCharacterSize(20);
    statusText.setFillColor(sf::Color::Green);
    statusText.setPosition({ (float)screenSize + 10, 150.f });
	return statusText;
}

sf::Text Game::boardEditText() const
{
    const sf::String arrow = U"►";

    sf::String rabbitStr = characterSelected == 1 ? sf::String(U"\t") + arrow + U"RABBIT\n" : sf::String(U"RABBIT\n");
    sf::String wolfStr = characterSelected == 2 ? sf::String(U"\t") + arrow + U"WOLF\n"   : sf::String(U"WOLF\n");
    sf::String wolfessStr = characterSelected == 3 ? sf::String(U"\t") + arrow + U"WOLFESS\n" : sf::String(U"WOLFESS\n");
    sf::String hedgeStr = characterSelected == 4 ? sf::String(U"\t") + arrow + U"HEDGE\n" : sf::String(U"HEDGE\n");
    sf::String clearStr = characterSelected == 0 ? sf::String(U"\t") + arrow + U"CLEAR\n" : sf::String(U"CLEAR\n");

    sf::Text boardEditText(fontGoogleSans);
    boardEditText.setString(sf::String(U"Add characters:\n") + rabbitStr + wolfStr + wolfessStr + hedgeStr + clearStr + sf::String(U"\n\nChange with UP/DOWN\nClick on the board to add/clear\nselected character"));
    boardEditText.setCharacterSize(20);
    boardEditText.setFillColor(sf::Color::Blue);
    boardEditText.setPosition({ (float)screenSize + 10, 350 });
    return boardEditText;
}

sf::Text Game::pauseText() const
{
	sf::Text pauseText(fontEmoji);
	pauseText.setString(U"\u23F8");
	pauseText.setCharacterSize((unsigned int)(screenSize*0.15));
	pauseText.setFillColor(sf::Color(0,0,0,153));
	pauseText.setOutlineColor(sf::Color(255,255,255,128));
	pauseText.setPosition({ (float)screenSize - screenSize * 0.15f, 0.f });
	return pauseText;
}

bool Game::loadTextures()
{
	bool success = true;

    auto pngData = loadResource(IDB_BG, L"PNG");
    if (pngData.empty() || !backgroundTexture.loadFromMemory(pngData.data(), pngData.size())) {
        std::cerr << "Failed to load background texture!" << std::endl;
		success = false;
    }

	pngData = loadResource(IDB_RABBIT, L"PNG");
    if (pngData.empty() || !rabbitTexture.loadFromMemory(pngData.data(), pngData.size())) {
        std::cerr << "Failed to load rabbit texture!" << std::endl;
        success = false;
    }

	pngData = loadResource(IDB_WOLF, L"PNG");
    if (pngData.empty() || !wolfTexture.loadFromMemory(pngData.data(), pngData.size())) {
        std::cerr << "Failed to load wolf texture!" << std::endl;
        success = false;
    }

	pngData = loadResource(IDB_HEDGE, L"PNG");
    if (pngData.empty() || !hedgeTexture.loadFromMemory(pngData.data(), pngData.size())) {
        std::cerr << "Failed to load hedge texture!" << std::endl;
        success = false;
    }

	pngData = loadResource(IDB_SKULL, L"PNG");
    if (pngData.empty() || !deathTexture.loadFromMemory(pngData.data(), pngData.size())) {
        std::cerr << "Failed to load death texture!" << std::endl;
        success = false;
    }

	pngData = loadResource(IDB_BLOOD, L"PNG");
    if (pngData.empty() || !eatTexture.loadFromMemory(pngData.data(), pngData.size())) {
        std::cerr << "Failed to load eat texture!" << std::endl;
        success = false;
    }

	pngData = loadResource(IDB_RHEART, L"PNG");
    if (pngData.empty() || !rabbitBreedTexture.loadFromMemory(pngData.data(), pngData.size())) {
        std::cerr << "Failed to load rabbit breed texture!" << std::endl;
        success = false;
    }

	pngData = loadResource(IDB_GHEART, L"PNG");
    if (pngData.empty() || !wolfBreedTexture.loadFromMemory(pngData.data(), pngData.size())) {
        std::cerr << "Failed to load wolf breed texture!" << std::endl;
        success = false;
    }

    backgroundTexture.setSmooth(true);
    rabbitTexture.setSmooth(true);
    wolfTexture.setSmooth(true);
	hedgeTexture.setSmooth(true);
	deathTexture.setSmooth(true);
    eatTexture.setSmooth(true);
	rabbitBreedTexture.setSmooth(true);
	wolfBreedTexture.setSmooth(true);

    return success;
}

Position Game::getFieldFromMousePosition(sf::Vector2i mousePos) const
{
	float thickness = lineThickness;
	float cellSize = (screenSize - (thickness * (boardSize - 1))) / boardSize;
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			int startX = i * (cellSize + thickness);
			int startY = j * (cellSize + thickness);
			if (mousePos.x >= startX && mousePos.x < startX + cellSize &&
				mousePos.y >= startY && mousePos.y < startY + cellSize) {
				return Position(i + 1, j + 1);
			}
		}
	}
    return Position(0,0);
}

void Game::handleEvents(sf::RenderWindow* window, const std::optional<sf::Event> * e)
{
    auto event = *e;
	// check for key presses
    if (event->is<sf::Event::KeyPressed>()) {
        switch (event->getIf<sf::Event::KeyPressed>()->code) {
        case sf::Keyboard::Key::Space: paused = !paused; break;
        case sf::Keyboard::Key::Right: frameDelay += 0.25f; break;
        case sf::Keyboard::Key::Left:  if (frameDelay > 0.25f) frameDelay -= 0.25f; break;
        case sf::Keyboard::Key::Down:  characterSelected = (characterSelected + 1) % 5; break;
        case sf::Keyboard::Key::Up:    characterSelected = (characterSelected + 4) % 5; break;
        default: break;
        }
    }

	// check for mouse clicks
    if (event->getIf<sf::Event::MouseButtonPressed>()) {
        if (paused && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
		    sf::Vector2i position(sf::Mouse::getPosition(*window));

			// add characters or clear field
		    Position fieldPos = getFieldFromMousePosition(position);
            switch(characterSelected) {
                case 0: // clear
                    while (GameCharacter* character = board.getCharacterAt(fieldPos)) {
                        board.removeCharacter(character);
                    }
                    break;
                case 1: // rabbit
                    board.addCharacter(fieldPos, CharacterType::RABBIT);
                    break;
                case 2: // wolf
                    board.addCharacter(fieldPos, CharacterType::WOLF);
                    break;
                case 3: // wolfess
                    board.addCharacter(fieldPos, CharacterType::WOLFESS);
                    break;
                case 4: // hedge
                    board.addCharacter(fieldPos, CharacterType::HEDGE);
                    break;
			}

            // clear all
            if (clearAllButton.getGlobalBounds().contains(window->mapPixelToCoords(position))) {
                board.gameCharacterList.clear();
            }
        }
    }

    if (clearAllButton.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
        const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand).value();
		window->setMouseCursor(cursor);
    }
    else {
        const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow).value();
        window->setMouseCursor(cursor);
    }
}

std::vector<uint8_t> Game::loadResource(WORD resourceId, const wchar_t* resourceType)
{
    HMODULE hModule = GetModuleHandleW(nullptr);
    HRSRC hRes = FindResourceW(hModule, MAKEINTRESOURCEW(resourceId), resourceType);
    if (!hRes) return {};

    HGLOBAL hData = LoadResource(hModule, hRes);
    if (!hData) return {};

    DWORD size = SizeofResource(hModule, hRes);
    void* pData = LockResource(hData);
    if (!pData || size == 0) return {};

    const uint8_t* begin = static_cast<const uint8_t*>(pData);
    return std::vector<uint8_t>(begin, begin + size);
}
