#pragma once
#include <SFML/Graphics.hpp>

class Animation {
private:
    float alpha;            // transparency of the texture
	float offsetY;          // vertical offset for floating effect
	int lifetime;           // remaining lifetime in frames
	int maxLifetime;        // maximum lifetime in frames
	sf::Texture& texture;   // reference to the texture used for the animation
public:
	sf::Vector2f position;  // position on the board
    Animation(sf::Vector2f pos, sf::Texture& texture);

	// Updates the animation state (position, transparency, lifetime)
    void update();
	// Draws the animation on the window
    void draw(sf::RenderWindow* window, unsigned int screenSize, unsigned int fieldNumber);
	// Checks if the animation has finished
    bool isFinished() const;
};