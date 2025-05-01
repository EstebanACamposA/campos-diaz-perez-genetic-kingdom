#include "Character.hpp"
#include <cmath>

Character::Character(sf::Vector2f startPosition, float speed) 
    : position(startPosition), speed(speed) {
    sprite.setRadius(10.f);
    sprite.setFillColor(sf::Color::Blue);
    sprite.setOrigin(10.f, 10.f);
}

// Moves character's postition a unit closer to the next destination in path if available.
void Character::update(float deltaTime) {
    // If character hasn't arrived at its last destination.
    if (currentTarget < path.size()) {
        sf::Vector2f direction = path[currentTarget] - position;
        // Magnitude of the direction vector.
        float distance = std::sqrt(direction.x*direction.x + direction.y*direction.y);
        
        // If distance is too short, character has already arrived at currentTarget and targets the next.
        if (distance < 5.f) {
            currentTarget++;
        } else {
            // Normalizes direction vector.
            direction /= distance;
            // Changes the characters postition value, doesn't draw jsut yet.
            position += direction * speed * deltaTime;
        }
    }
}

// Takes a newPath and stores it into this.path.
// newPath gives (x,y) coordinates without taking the tileSize (actual position in the screen) into account,
// only the coordinates as the TileMap(?) sees them.
// Also adds half a tile to each point in path so that it's placed in the middle of the tile.
void Character::setPath(const std::vector<sf::Vector2i>& newPath, float tileSize) {
    path.clear();
    for (const auto& point : newPath) {
        path.emplace_back(
            point.x * tileSize + tileSize/2,    // adding, not substracting.
            point.y * tileSize + tileSize/2
        );
    }
    currentTarget = 0;  // Resets currentTarget that iterates along the path in update(float deltaTime)
}

void Character::draw(sf::RenderWindow& window) {
    sprite.setPosition(position);
    window.draw(sprite);
}

sf::Vector2f Character::getPosition() const {
    return position;
}