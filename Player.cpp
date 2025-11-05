#include "Player.h"
#include <iostream>

Player::Player() : m_playerSprite(this->m_playerTexture), GRAVITY(1200.f), velocity(0.f), position(100.f),
                   JUMP_IMPULSE(-450.f) {
    if (!m_playerTexture.loadFromFile("assets/ring.png")) {
        std::cerr << "Could not load ring.png" << std::endl;
    }

    m_playerSprite.setTexture(m_playerTexture, true);
    m_playerSprite.setScale({0.25f, 0.25f});

    m_playerSprite.setPosition({240, position});
}

void Player::draw(sf::RenderWindow &window) const {
    window.draw(m_playerSprite);
}

void Player::jump() {
    velocity = JUMP_IMPULSE;
}

void Player::update(sf::RenderWindow& window,sf::Time dt) {
    velocity += GRAVITY*dt.asSeconds();
    position += velocity*dt.asSeconds();
    m_playerSprite.setPosition({m_playerSprite.getPosition().x,position});
}

sf::FloatRect Player::getBounds() const {
    sf::FloatRect bounds = m_playerSprite.getGlobalBounds();
    float inset = 10.f;

    bounds.position.x += inset;
    bounds.position.y += inset;
    bounds.size.x -= (inset * 2.f);
    bounds.size.y -= (inset * 2.f);

    return bounds;
}

void Player::reset() {
    position = 350.f;
    velocity = 0.f;

    m_playerSprite.setPosition({240, position});
}
