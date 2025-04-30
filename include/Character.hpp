#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Character {
public:
    Character(sf::Vector2f startPosition, float speed = 100.f);
    void update(float deltaTime);
    void setPath(const std::vector<sf::Vector2i>& newPath, float tileSize);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape sprite;
    std::vector<sf::Vector2f> path;
    float speed;
    size_t currentTarget = 0;
    sf::Vector2f position;
};