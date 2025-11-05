#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void draw(sf::RenderWindow& window) const;
    void update(sf::RenderWindow& window,sf::Time dt);
    void jump();
    sf::FloatRect getBounds() const;
    void reset();

private:
    sf::Texture m_playerTexture;
    sf::Sprite m_playerSprite;

    const float GRAVITY;
    float velocity;
    float position;
    const float JUMP_IMPULSE;
};
