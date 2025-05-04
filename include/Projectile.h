// #include <cmath>
#include <iostream>
#include <vector>
#include <memory>
#include <vector>
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
    Projectile(sf::Vector2f startPosition, std::shared_ptr<Character> target_character, float speed = 200.f); // 200.f is default speed. 
    bool update(float deltaTime);
    // void setPath(const std::vector<sf::Vector2i>& newPath, float tileSize);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

    // bool destroyed = false;

private:
    // SFML and movement:
    sf::CircleShape sprite;
    // std::vector<sf::Vector2f> path;
    float speed;
    // size_t currentTarget = 0;
    sf::Vector2f position;
    // float closest_distance_to_target;




};














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



// Special abilites.
    // Posion ignores armor. Deals set damage per frame.
    // Frost is permanent or lasts a lot? Has to be kinda bad because magic has no weaknesses.
    // Shock lasts a little. 