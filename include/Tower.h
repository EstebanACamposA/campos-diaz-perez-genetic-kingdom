#ifndef TOWER_H
#define TOWER_H


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

#include <SFML/Graphics.hpp>

class Tower
{
public:
    // std::shared_ptr<Character> target_character;
    float tileSize;
    // Basics and movement:
    Tower(sf::Vector2f startPosition, int tower_type, int tower_level, float tile_size, const sf::Texture& texture);

    // {0,1,2} -> {pierce, magic, siege}
    int tower_type;
    float damage;
    // Returns true if a bullet must be fired.
    // Fire a bullet taking this object's damage.
    // Fires every set amount of frames depending on type.
    bool update(float deltaTime);


    void draw(sf::RenderWindow& window);
    
    sf::Vector2f getPosition() const;

    void upgrade();

    float fire_range;

    float projectile_speed;

private:
    /// Testing visuals ///
    // // SFML and movement:
    // sf::CircleShape sprite;
    // // std::vector<sf::Vector2f> path;
    /// Testing visuals ///

    // period between shots in frames.
    int period;
    int period_progress = 0;

    

    sf::Vector2f position;
    // float closest_distance_to_target;

    sf::Color tower_colors[3];

    // SFML Texture
    // sf::Texture texture;
    sf::Sprite sprite_texture;
    sf::Color upgrade_color;



};






#endif