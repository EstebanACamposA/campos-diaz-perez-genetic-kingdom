#include "Projectile.h"
#include <cmath>
#include <iostream>

// Base constructor.
Projectile::Projectile(sf::Vector2f startPosition, std::shared_ptr<Character> target_character, float speed) 
    : position(startPosition), target_character(target_character), speed(speed) {
    sprite.setRadius(5.f);
    sprite.setFillColor(sf::Color::Green);
    sprite.setOrigin(5.f, 5.f);
}

// Moves Projectile's postition a unit closer to it's target.
// Returns true if the projectile has hit the target and must be destroyed.
bool Projectile::update(float deltaTime)
{
    sf::Vector2f displacement = target_character->getPosition() - position;
    // Magnitude of displacement.
    float distance = std::sqrt(displacement.x*displacement.x + displacement.y*displacement.y);
    // if (closest_distance_to_target)
    // {
    //     /* code */
    // }
    if (distance <= speed*deltaTime)
    {
        std::cout << "distance <= speed*deltaTime   OJO CUIDADO!!!!!!!!!!!!" << std::endl;
        target_character->magic_damage += 20.f;
        return true;
        // Returning true will make the GameManager remove this projectile.
    }
    std::cout << "distance = " << distance << std::endl;
    std::cout << "speed = " << speed << std::endl;
    std::cout << "deltaTime = " << deltaTime << std::endl;
    std::cout << "speed*deltaTime = " << speed*deltaTime << std::endl;
    std::cout << std::endl;
    
    position += displacement/distance * speed * deltaTime;
    return false;
}

// currently called by main 
void Projectile::draw(sf::RenderWindow& window) {
    // There could be a VFX related position that follows the actuall position depending on the distance between the two and show this one.
    // This would avoid sharp turns.
    sprite.setPosition(position);
    window.draw(sprite);
}

sf::Vector2f Projectile::getPosition() const {
    return position;
}

