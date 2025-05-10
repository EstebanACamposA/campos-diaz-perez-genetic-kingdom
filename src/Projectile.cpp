#include "Projectile.h"
#include <cmath>
#include <iostream>

// Base constructor.
// Projectile::Projectile(sf::Vector2f startPosition, std::shared_ptr<Character> target_character, float speed) 
//     : position(startPosition), target_character(target_character), speed(speed) {
//     sprite.setRadius(5.f);
//     sprite.setFillColor(sf::Color::Green);
//     sprite.setOrigin(5.f, 5.f);
// }



// Constructor for damage and tower_type.
Projectile::Projectile(sf::Vector2f startPosition, std::shared_ptr<Character> target_character, float speed, float damage) 
    : position(startPosition), target_character(target_character), speed(speed), damage(damage) {
    sprite.setRadius(5.f);
    sprite.setFillColor(sf::Color::Green);
    sprite.setOrigin(5.f, 5.f);

    target_original_position = target_character->getPosition();
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
        // std::cout << "distance <= speed*deltaTime -> projectile marked for deletion." << std::endl;
        // target_character->magic_damage += 20.f;
        // Based on the damage of the projectile's type pierce, magic, siege (0.75, 2.00, 2.50)
        target_character->magic_damage += 40.f;

        return true;
        // Returning true will make the GameManager remove this projectile.
    }
    // std::cout << "distance = " << distance << std::endl;
    // std::cout << "speed = " << speed << std::endl;
    // std::cout << "deltaTime = " << deltaTime << std::endl;
    // std::cout << "speed*deltaTime = " << speed*deltaTime << std::endl;
    // std::cout << std::endl;
    
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

float Projectile::getDamage()
{
    return damage;
}

/// Arrows ///
// Moves in a straight line until collinding with the position the enemy had when the arrow was shot.
// Damages the target enemy (even if it may not have landed on target).
// Poison Arrow permanently inflicts poison effect on an enemy. 60 dps.
bool Arrow::update(float deltaTime)
{
    sf::Vector2f displacement = target_original_position - position;
    // Magnitude of displacement.
    float distance = std::sqrt(displacement.x*displacement.x + displacement.y*displacement.y);
    
    if (distance <= speed*deltaTime)
    {
        target_character->pierce_damage += damage;

        return true;
        // Returning true will make the GameManager remove this projectile.
    }
    
    position += displacement/distance * speed * deltaTime;
    return false;
}
bool PoisonArrow::update(float deltaTime)
{
    sf::Vector2f displacement = target_original_position - position;
    // Magnitude of displacement.
    float distance = std::sqrt(displacement.x*displacement.x + displacement.y*displacement.y);
    
    if (distance <= speed*deltaTime)
    {
        target_character->pierce_damage += damage;
        target_character->posion_arrow_effect = true;   // Permanently inflicts poison effect on an enemy. 60 dps.

        return true;
        // Returning true will make the GameManager remove this projectile.
    }
    
    position += displacement/distance * speed * deltaTime;
    return false;
}

/// Magic ///
// Magic tracking is the default. Follows the enemy until impact.
// FrostOrb slows down enemies. Halves speed.
bool FrostOrb::update(float deltaTime)
{
    sf::Vector2f displacement = target_character->getPosition() - position;
    // Magnitude of displacement.
    float distance = std::sqrt(displacement.x*displacement.x + displacement.y*displacement.y);
    if (distance <= speed*deltaTime)
    {
        target_character->magic_damage += damage;
        target_character->ApplyFrostOrbEffect();
        return true;
        // Returning true will make the GameManager remove this projectile.
    }
    position += displacement/distance * speed * deltaTime;
    return false;
}

/// Siege ///
// Siege tracking is the same as arrows.
// Shock Shell briefly inverts enemies direction of movement. Makes speed negative.
bool ExplosiveShell::update(float deltaTime)
{
    sf::Vector2f displacement = target_original_position - position;
    // Magnitude of displacement.
    float distance = std::sqrt(displacement.x*displacement.x + displacement.y*displacement.y);
    
    if (distance <= speed*deltaTime)
    {
        // target_character->siege_damage += damage;
        explosive_damage = true;    // This flags signals TileMap to handle the AoE damage.

        return true;
        // Returning true will make the GameManager remove this projectile.
    }
    
    position += displacement/distance * speed * deltaTime;
    return false;
}
bool ShockShell::update(float deltaTime)
{
    sf::Vector2f displacement = target_original_position - position;
    // Magnitude of displacement.
    float distance = std::sqrt(displacement.x*displacement.x + displacement.y*displacement.y);
    
    if (distance <= speed*deltaTime)
    {
        // target_character->siege_damage += damage;
        explosive_damage = true;            // This flags signals TileMap to handle the AoE damage.
        apply_shock_shell_effect = true;    // This flags signals TileMap to handle the AoE damage.
        return true;
        // Returning true will make the GameManager remove this projectile.
    }
    
    position += displacement/distance * speed * deltaTime;
    return false;
}








// class Arrow : public Projectile {
//     public:
        
//     };

// class MagicOrb : public Projectile {
//     public:
        
//     };

// class ExplosiveShell : public Projectile {
//     public:
        
//     };

// // special ability
// class PoisonArrow : public Projectile {
//     public:
        
//     };

// class FrostOrb : public Projectile {
//     public:
        
//     };

// class ShockShell : public Projectile {
//     public:
        
//     };



