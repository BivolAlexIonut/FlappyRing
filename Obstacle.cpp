#include "Obstacle.h"

const float OBSTACLE_SCALE_X = 0.5f;
const float OBSTACLE_SCALE_Y = 0.5f;
Obstacle::Obstacle(sf::Texture &texture, sf::Vector2f position,bool isTopPipe)
    : m_obstacleSprite(this->m_obstacleTexture),
        m_currentFrame(0), m_frameCount(10), m_frameWidth(294), m_frameHeight(512),m_isScored(false),m_isTopPipe(isTopPipe) {
    //setez textura
    m_obstacleSprite.setTexture(texture,true);
    m_animationRect = sf::IntRect({0,0},{m_frameWidth,m_frameHeight});
    m_obstacleSprite.setTextureRect(m_animationRect);
    m_obstacleSprite.setPosition(position);

    m_frameDuration = sf::seconds(0.1f);

    if (isTopPipe) {
        m_obstacleSprite.setOrigin({0.f, (float)m_frameHeight});
        m_obstacleSprite.setScale({OBSTACLE_SCALE_X, -OBSTACLE_SCALE_Y-0.5f});
    } else {
        m_obstacleSprite.setScale({OBSTACLE_SCALE_X, OBSTACLE_SCALE_Y+0.5f});
    }
}

void Obstacle::update(sf::RenderWindow &window, float scrollSpeed,sf::Time dt) {
    m_obstacleSprite.move({-scrollSpeed*dt.asSeconds(),0});

    if (m_animationTimer.getElapsedTime() > m_frameDuration) {
        m_currentFrame++;

        if (m_currentFrame >= m_frameCount) {
            m_currentFrame = 0;
        }
        m_animationRect.position.x = m_currentFrame * m_frameWidth;
        m_obstacleSprite.setTextureRect(m_animationRect);

        m_animationTimer.restart();
    }
}

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(m_obstacleSprite);
}

float Obstacle::getPositionX() const {
    return m_obstacleSprite.getPosition().x;
}

sf::FloatRect Obstacle::getBounds() const {
    sf::FloatRect bounds = m_obstacleSprite.getGlobalBounds();
    float horizontalInset = 20.f;
    bounds.position.x += horizontalInset;
    bounds.size.x -= (horizontalInset*2.f);
    return bounds;
}

bool Obstacle::isAlreadyScored() const {
    return m_isScored;
}

void Obstacle::markAsScored() {
    m_isScored = true;
}

bool Obstacle::isTopPipe() const {
    return m_isTopPipe;
}