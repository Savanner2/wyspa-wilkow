#include "DeathAnimation.h"
#include <iostream>

DeathAnimation::DeathAnimation(sf::Vector2f pos, sf::Texture& texture)
    : Animation(pos, 120, texture), alpha(255.f), offsetY(0.f) {}

void DeathAnimation::update() {
    lifetime--;
    offsetY -= 1.f;  // Move up
    alpha -= 255/(float)maxLifetime;   // Fade out
}

void DeathAnimation::draw(sf::RenderWindow* window, unsigned int screenSize, unsigned int fieldNumber) {
    float fieldSize = screenSize / static_cast<float>(fieldNumber);
    
    sf::Sprite sprite(texture);
    
    // Set scale
    float scale = fieldSize / sprite.getLocalBounds().size.x * 0.5f;
    sprite.setScale({ scale, scale });
    
    // Set position with offset
    sprite.setPosition({ position.x * fieldSize + 0.25f * fieldSize, position.y * fieldSize + 0.25f * fieldSize + offsetY});
    
    // Set opacity
    sprite.setColor(sf::Color(255, 255, 255, alpha > 0 ? alpha : 0.f));
    
    window->draw(sprite);
}