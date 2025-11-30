#pragma once
#include "Animation.h"

class DeathAnimation : public Animation {
    private:
    float alpha;
    float offsetY;
public:
    DeathAnimation(sf::Vector2f pos, sf::Texture& texture);
    
    void update() override;
    void draw(sf::RenderWindow* window, unsigned int screenSize, unsigned int fieldNumber) override;
};