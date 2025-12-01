#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <iostream>

Animation::Animation(Position pos, sf::Texture& texture)
    : position(pos), lifetime(120), maxLifetime(lifetime), texture(texture), alpha(255.f), offsetY(0.f) {
}

bool Animation::isFinished() const {
    return lifetime <= 0;
}

void Animation::update() {
	lifetime--;     // Decrease lifetime
    offsetY -= 1.f;  // Move up
    alpha -= 255 / (float)maxLifetime;   // Fade out
}

void Animation::draw(sf::RenderWindow* window, unsigned int screenSize, unsigned int fieldNumber) {
    float fieldSize = screenSize / static_cast<float>(fieldNumber);

    sf::Sprite sprite(texture);

    // Set scale
    float scale = fieldSize / sprite.getLocalBounds().size.x * 0.5f;
    sprite.setScale({ scale, scale });

    // Set position with offset
    sprite.setPosition({ position.x * fieldSize - 0.75f * fieldSize, position.y * fieldSize - 0.75f * fieldSize + offsetY });

    // Set opacity
    sprite.setColor(sf::Color(255, 255, 255, alpha > 0 ? alpha : 0.f));

    window->draw(sprite);
}