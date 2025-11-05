#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    Obstacle(sf::Texture& texture, sf::Vector2f position,bool isTopPipe);
    void update(sf::RenderWindow& window,float scrollSpeed,sf::Time dt);
    void draw(sf::RenderWindow& window);
    float getPositionX() const;
    sf::FloatRect getBounds() const;
    bool isAlreadyScored() const;
    void markAsScored();
    bool isTopPipe() const;
    ~Obstacle() = default;
private:
    sf::Sprite m_obstacleSprite;
    sf::Texture m_obstacleTexture;
    sf::IntRect m_animationRect;
    int m_currentFrame;
    sf::Clock m_animationTimer;
    sf::Time m_frameDuration;
    int m_frameCount;
    int m_frameWidth;
    int m_frameHeight;
    bool m_isTopPipe;
    bool m_isScored;
};