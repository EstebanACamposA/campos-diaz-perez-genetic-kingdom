#include "Character.hpp"
#include <cmath>

Character::Character(sf::Vector2f startPosition, float speed) 
    : position(startPosition), speed(speed) {
    sprite.setRadius(10.f);
    sprite.setFillColor(sf::Color::Blue);
    sprite.setOrigin(10.f, 10.f);
}

void Character::update(float deltaTime) {
    if (currentTarget < path.size()) {
        sf::Vector2f direction = path[currentTarget] - position;
        float distance = std::sqrt(direction.x*direction.x + direction.y*direction.y);
        
        if (distance < 5.f) {
            currentTarget++;
        } else {
            direction /= distance;
            position += direction * speed * deltaTime;
        }
    }
}

void Character::setPath(const std::vector<sf::Vector2i>& newPath, float tileSize) {
    path.clear();
    for (const auto& point : newPath) {
        path.emplace_back(
            point.x * tileSize + tileSize/2,
            point.y * tileSize + tileSize/2
        );
    }
    currentTarget = 0;
}

void Character::draw(sf::RenderWindow& window) {
    sprite.setPosition(position);
    window.draw(sprite);
}

sf::Vector2f Character::getPosition() const {
    return position;
}