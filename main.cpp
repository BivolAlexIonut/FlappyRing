#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Obstacle.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "FlappyRing");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("assets/arial.ttf")) {
        std::cerr << "Could not load font" << std::endl;
        return -1;
    }
    int score = 0;
    sf::Text scoreText(font, "Score: " + std::to_string(score), 32);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition({10.f, 10.f});

    sf::Text gameOverText(font, "Game Over", 64);
    gameOverText.setFillColor(sf::Color::Red);

    sf::Text restartText(font, "Press Enter to Restart", 32);
    restartText.setFillColor(sf::Color::Black);

    sf::Texture obstacleTexture;
    if (!obstacleTexture.loadFromFile("assets/Obstacle.png")) {
        std::cerr << "Could not load obstacle texture" << std::endl;
        return -1;
    }
    std::vector<Obstacle> obstacleList;

    sf::Texture backGroundTexture;
    if (!backGroundTexture.loadFromFile("assets/background.png")) {
        std::cerr << "Could not load background texture" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backGroundTexture);
    backgroundSprite.setTexture(backGroundTexture);

    float scaleX = static_cast<float>(window.getSize().x) / backGroundTexture.getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / backGroundTexture.getSize().y;
    backgroundSprite.setScale({scaleX, scaleY});
    backgroundSprite.setPosition({0, 0});

    float bgWidth = backGroundTexture.getSize().x * scaleX;

    sf::Sprite backgroundSprite2(backgroundSprite);
    backgroundSprite2.setTexture(backGroundTexture);
    backgroundSprite2.setScale({scaleX, scaleY});
    backgroundSprite2.setPosition({bgWidth, 0});

    const float SCROLL_SPEED = 200.f;
    const float GAP_SIZE = 250.f;
    const float GAP_Y_OFFSET = 384.f;
    const float GAP_Y_VARIATION = 200.f;

    Player player;
    bool isAlive = true;
    sf::Clock clock;

    sf::Clock obstacleSpawnTimer;
    sf::Time spawnInterval = sf::seconds(1.5f);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (window.isOpen()) {
        sf::Time dt = clock.restart();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (isAlive && event->is<sf::Event::KeyPressed>()) {
                auto keyPressedEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyPressedEvent && keyPressedEvent->code == sf::Keyboard::Key::Space) {
                    player.jump();
                }
            }
            if (!isAlive && event->is<sf::Event::KeyPressed>()) {
                auto keyPressedEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyPressedEvent && keyPressedEvent->code == sf::Keyboard::Key::Enter) {
                    isAlive = true;
                    player.reset();
                    obstacleList.clear();
                    obstacleSpawnTimer.restart();

                    score = 0;
                    scoreText.setString("Score: " + std::to_string(score));

                    backgroundSprite.setPosition({0, 0});
                    backgroundSprite2.setPosition({bgWidth, 0});
                }
            }
        }

        if (isAlive) {
            player.update(window, dt);
            sf::FloatRect playerBounds = player.getBounds();
            if (playerBounds.position.y < 0.f) {
                isAlive=false;
            }
            if (playerBounds.position.y + playerBounds.size.y > window.getSize().y) {
                isAlive = false;
            }

            if (obstacleSpawnTimer.getElapsedTime() > spawnInterval) {
                obstacleSpawnTimer.restart();

                float randomOffset = (static_cast<float>(std::rand() % (int)GAP_Y_VARIATION)) - (GAP_Y_VARIATION / 2);
                float gapCenterY = GAP_Y_OFFSET + randomOffset;

                float topPipeY = gapCenterY - (GAP_SIZE / 2);
                float bottomPipeY = gapCenterY + (GAP_SIZE / 2);

                float xPos = (float)window.getSize().x;

                Obstacle topObstacle(obstacleTexture, {xPos, topPipeY -480.f}, true);
                Obstacle bottomObstacle(obstacleTexture, {xPos, bottomPipeY}, false);
                obstacleList.push_back(topObstacle);
                obstacleList.push_back(bottomObstacle);
            }

            for (int i = obstacleList.size() - 1; i >= 0; --i) {
                obstacleList[i].update(window,SCROLL_SPEED,dt);

                sf::FloatRect PlayerBounds = player.getBounds();
                sf::FloatRect obstacleBounds = obstacleList[i].getBounds();

                if (PlayerBounds.findIntersection(obstacleBounds)) {
                    isAlive = false;
                }

                float playerX = player.getBounds().position.x;

                if (!obstacleList[i].isTopPipe() &&
                    !obstacleList[i].isAlreadyScored() &&
                    obstacleList[i].getPositionX() < playerX)
                {
                    score++;
                    obstacleList[i].markAsScored();
                    scoreText.setString("Score: " + std::to_string(score));
                }

                if (obstacleList[i].getPositionX() < -300) {
                    obstacleList.erase(obstacleList.begin() + i);
                }
            }

            backgroundSprite.move({-SCROLL_SPEED * dt.asSeconds(),0});
            backgroundSprite2.move({-SCROLL_SPEED * dt.asSeconds(),0});

            if (backgroundSprite.getPosition().x <= -bgWidth) {
                backgroundSprite.setPosition({backgroundSprite2.getPosition().x + bgWidth, 0});
            }
            if (backgroundSprite2.getPosition().x <= -bgWidth) {
                backgroundSprite2.setPosition({backgroundSprite.getPosition().x + bgWidth, 0});
            }
        }

        window.clear(sf::Color::White);
        window.draw(backgroundSprite);
        window.draw(backgroundSprite2);

        for (auto& obstacle : obstacleList) {
            obstacle.draw(window);
        }
        player.draw(window);
        window.draw(scoreText);

        if (!isAlive) {
            float centerX = window.getSize().x / 2.f;
            float centerY = window.getSize().y / 2.f;

            gameOverText.setPosition({centerX - gameOverText.getGlobalBounds().size.x / 2.f, centerY - 100.f});
            restartText.setPosition({centerX - restartText.getGlobalBounds().size.x / 2.f, centerY + 20.f});

            window.draw(gameOverText);
            window.draw(restartText);
        }

        window.display();
    }
}