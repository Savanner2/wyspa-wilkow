#pragma once
#include <SFML/Graphics.hpp>

class Animation {
private:
    float alpha;
    float offsetY;
    int lifetime;
    int maxLifetime;
    sf::Texture& texture;

public:
    sf::Vector2f position;
    Animation(sf::Vector2f pos, sf::Texture& texture);

    void update();
    void draw(sf::RenderWindow* window, unsigned int screenSize, unsigned int fieldNumber);
    
    bool isFinished() const;
};