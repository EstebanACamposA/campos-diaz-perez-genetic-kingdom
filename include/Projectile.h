#ifndef PROJECTILE_H
#define PROJECTILE_H


// #include <cmath>
#include <iostream>
#include <vector>
#include <memory>
// #include <vector>
// #include <cstring>
// #include <cstdint>
// #include <iomanip>

// #include <random>
// #include <algorithm>

#include "Character.hpp"

class Projectile
{
public:
    std::shared_ptr<Character> target_character;
    float tileSize;
    // Basics and movement:
    // Projectile(sf::Vector2f startPosition, std::shared_ptr<Character> target_character, float speed = 200.f); // 200.f is default speed. 
    // // Make a constructor that takes the tower_type.
    Projectile(sf::Vector2f startPosition, std::shared_ptr<Character> target_character, float speed, float damage);
    
    bool virtual update(float deltaTime);
    // void setPath(const std::vector<sf::Vector2i>& newPath, float tileSize);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

    // bool destroyed = false;

    // Inherited virtual destructor.
    virtual ~Projectile() = default;  // Use virtual for safe polymorphic deletion

    bool explosive_damage = false;
    bool apply_shock_shell_effect = false;

    float getDamage();
        

protected:
    // SFML and movement:
    sf::CircleShape sprite;
    // std::vector<sf::Vector2f> path;
    float speed;
    // size_t currentTarget = 0;
    sf::Vector2f position;
    // float closest_distance_to_target;
    sf::Vector2f target_original_position;

    float damage;



};














class Arrow : public Projectile {
    public:
        bool update(float deltaTime) override;
        using Projectile::Projectile;   // Inherit the constructor.
    };
// special ability
class PoisonArrow : public Projectile {
    public:
        bool update(float deltaTime) override;
        using Projectile::Projectile;   // Inherit the constructor.
    };
// special ability
class FrostOrb : public Projectile {
    public:
        bool update(float deltaTime) override;
        using Projectile::Projectile;   // Inherit the constructor.
    };
class ExplosiveShell : public Projectile {
    public:
        bool update(float deltaTime) override;
        using Projectile::Projectile;   // Inherit the constructor.
    };
// special ability
class ShockShell : public Projectile {
    public:
        bool update(float deltaTime) override;
        using Projectile::Projectile;   // Inherit the constructor.
    };



// Magic Orb behaves like the base class. May change graphically.
class MagicOrb : public Projectile {
    public:
        using Projectile::Projectile;   // Inherit the constructor.
    };





// Special abilites.
    // Posion ignores armor. Deals set damage per frame.
    // Frost is permanent or lasts a lot? Has to be kinda bad because magic has no weaknesses.
    // Shock lasts a little. 









#endif