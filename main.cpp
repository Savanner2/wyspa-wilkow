#include <SFML/Graphics.hpp>
#include<iostream>
#include <vector>
#include <algorithm>

#include "Board.h"
#include "Game.h"
#include "Rabbit.h"
#include "Wolf.h"
#include "Animation.h"
#include "Hedge.h"

int main()
{
    srand(time(NULL));
    unsigned int boardSize;
    unsigned int screenSize = 900;

    // select board size
	std::cout << "Enter board size (suggested 10-20): ";
    std::cin >> boardSize;
    while (boardSize < 2 || boardSize > 30) {
		std::cout << "Invalid board size! Please enter a size between 2 and 30: ";
        std::cin >> boardSize;
    }

    // initialize game
	Game game(boardSize, screenSize);

    // create window
    sf::RenderWindow window(sf::VideoMode({ screenSize + 400, screenSize }), "Wyspa Wilków");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        // pool events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // handle key presses
            game.handleEvents(&window, &event);
        }

        // update all animations
        game.board.updateAnimations();

        // check if game should tick
        if (game.tick == 0 && !game.paused) {
            std::cout << "\033[H\033[2J" << std::flush;
			/*size_t initialCharacterCount = game.board.gameCharacterList.size();
			for (int i = 0; i < initialCharacterCount; i++)
            {
                GameCharacter* character = game.board.gameCharacterList[i];*/

            // loop through every character
            for(GameCharacter* character : game.board.gameCharacterList) {
				// skip characters already marked for death
                if (std::find(game.board.deadCharacters.begin(), game.board.deadCharacters.end(), character) != game.board.deadCharacters.end()) continue;

                // handle wolves
                if (character->type == CharacterType::WOLF || character->type == CharacterType::WOLFESS) {
				    bool hasActed = false;
                    // wolf death logic
                    Wolf* wolf = dynamic_cast<Wolf*>(character);
                    if (wolf->fat <= 0) {
						game.board.deadCharacters.push_back(character);
                        game.board.animationList.push_back(new Animation(
                            sf::Vector2f(
                                character->pos.x,
                                character->pos.y
                            ),
                            game.deathTexture));
                        std::cout << "A wolf has died at " << "(" << wolf->pos.x << ", " << wolf->pos.y << ")" << std::endl;
						continue;
                    } 

                    // wolf breeding logic
                    if (game.board.wolfBreedPossible(character->pos) && !hasActed) {
						game.board.newCharacters.push_back(new Wolf(Position(character->pos.x, character->pos.y)));
						game.board.animationList.push_back(new Animation(
							sf::Vector2f(
								character->pos.x,
								character->pos.y
							),
							game.wolfBreedTexture));
						std::cout << "A wolf has bred at (" << character->pos.x << ", " << character->pos.y << ")" << std::endl;
						hasActed = true;
                    }

                    // wolf move logic
                    if (game.board.canMove(character) && !hasActed) {
						sf::Vector2i move = game.board.randomMove(character);
						int deltaX = move.x;
						int deltaY = move.y;

						Wolf* wolf = dynamic_cast<Wolf*>(character);
						// check if wolf has enough fat to breed
                        if (wolf->fat >= game.board.breedFatThreshold && character->type == CharacterType::WOLF) {
                            Wolf* wolfess = dynamic_cast<Wolf*>(game.board.getClosestCharacter(character->pos, CharacterType::WOLFESS));
                            // move towards nearby wolfess if exists
                            if (wolfess != nullptr) {
                                int newDeltaX = std::clamp((wolfess->pos.x - character->pos.x), -1, 1);
                                int newDeltaY = std::clamp((wolfess->pos.y - character->pos.y), -1, 1);
                                if (game.board.canMoveTo(Position(wolf->pos.x + newDeltaX, wolf->pos.y + newDeltaY), true)) {
								    deltaX = newDeltaX;
                                    deltaY = newDeltaY;
                                }
                            }
                        }
						// move towards rabbit if low on fat
                        else {
                            Rabbit* rabbit = dynamic_cast<Rabbit*>(game.board.getClosestCharacter(character->pos, CharacterType::RABBIT));
                            if (rabbit != nullptr) {
                                int newDeltaX = std::clamp((rabbit->pos.x - character->pos.x), -1, 1);
                                int newDeltaY = std::clamp((rabbit->pos.y - character->pos.y), -1, 1);
                                if (game.board.canMoveTo(Position(wolf->pos.x + newDeltaX, wolf->pos.y + newDeltaY), true))
                                {
                                    deltaX = newDeltaX;
                                    deltaY = newDeltaY;
									std::cout << "Wolf at (" << wolf->pos.x << ", " << wolf->pos.y << ") is moving towards rabbit at (" << rabbit->pos.x << ", " << rabbit->pos.y << ")" << std::endl;
                                }
                            }
                        }
                        character->Move(deltaX, deltaY);
                        
                    }

                    // wolf eating logic
                    if (game.board.feastTime(character->pos)) {
                        Wolf* wolf = dynamic_cast<Wolf*>(game.board.getCharacterAt(character->pos, CharacterType::WOLF));
                        if (wolf == nullptr) {
                            wolf = dynamic_cast<Wolf*>(game.board.getCharacterAt(character->pos, CharacterType::WOLFESS));
                        }
                        Rabbit* rabbit = dynamic_cast<Rabbit*>(game.board.getCharacterAt(character->pos, CharacterType::RABBIT));

                        if (std::find(game.board.deadCharacters.begin(), game.board.deadCharacters.end(), rabbit) == game.board.deadCharacters.end()) {
                            game.board.deadCharacters.push_back(rabbit);
                            game.board.animationList.push_back(new Animation(
                                sf::Vector2f(
                                    character->pos.x,
                                    character->pos.y
                                ),
                                game.eatTexture));
                            wolf->fat += 1;
                            std::cout << "A wolf has eaten a rabbit at " << "(" << wolf->pos.x << ", " << wolf->pos.y << ")" << std::endl;
                        }
                    }
                }
                // handle rabbits
                else if(character->type == CharacterType::RABBIT) {
                    bool idle = (rand() % 100) < (game.rabbitIdleChance * 100);
                    bool breed = (rand() % 100) < (game.rabbitBreedChance * 100);

                    if (!idle) {
                        // breeding logic
                        if (breed && game.board.rabbitBreedPossible(Position(character->pos.x, character->pos.y))) {
							game.board.newCharacters.push_back(new Rabbit(Position(character->pos.x, character->pos.y)));
							game.board.animationList.push_back(new Animation(
								sf::Vector2f(
									character->pos.x,
									character->pos.y
								),
								game.rabbitBreedTexture));
							std::cout << "A rabbit has bred at (" << character->pos.x << ", " << character->pos.y << ")" << std::endl;
							
                        }
                        // moving logic
                        else if (game.board.canMove(character)) {
							sf::Vector2i move = game.board.randomMove(character);
                            int deltaX = move.x;
                            int deltaY = move.y;

                            character->Move(deltaX, deltaY);
                            //std::cout << ((dynamic_cast<Rabbit*>(character) == nullptr) ? "Wolf" : "Rabbit") << " moved to (" << character->pos.x << ", " << character->pos.y << ")" << std::endl;
                        }
                    }
                }
                

            }

            // update board
			game.board.applyDeathsAndBirths();

            std::cout << "\n\nCharacter count: " << game.board.gameCharacterList.size() << std::endl;
        }
        game.tick = (game.tick + 1) % game.speed;
		game.speed = 60 / game.frameDelay;

        window.clear();

		// draw board background
        game.drawBackground(&window, screenSize, boardSize);
		
        // draw board grid
        game.drawGrid(&window, screenSize, boardSize);
		
        // draw characters
        game.drawCharacters(&window);
		
		// draw animations
        game.board.drawAnimations(&window, screenSize, boardSize);

        // draw text and controls
        game.drawControls(&window);

        window.display();
    }
}