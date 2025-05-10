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

#include "Tower.h"



// std::shared_ptr<Character> target_character;
float tileSize;
Tower::Tower(sf::Vector2f startPosition, int tower_type, int tower_level, float fire_range) 
: position(startPosition), tower_type(tower_type){
// Visuals:
    sprite.setRadius(10.f);

    sf::Color tower_colors[3] = {
        sf::Color(190, 190, 255),
        sf::Color(255, 170, 210),
        sf::Color(255, 190, 150)
    };
    sprite.setFillColor(tower_colors[tower_type]);
    sprite.setOrigin(10.f, 10.f);

    sprite.setPosition(position);
    // Stats. Depend on level and type
    // Period.
    int towers_period_by_type[3] = {60, 120, 240};
    float towers_period_multiplier_by_level[3] = {0.64f, 0.8f, 1.0f};
    period = towers_period_by_type[tower_type];
    period = (int) (period*towers_period_multiplier_by_level[tower_level]);
    // Damage
    float towers_damage_by_type[3] = {7.5f, 20.f, 25.f};
    float towers_damage_multiplier_by_level[3] = {1.f, 1.2f, 1.44f};
    damage = towers_damage_by_type[tower_type];
    damage *= towers_damage_multiplier_by_level[tower_level];
    // Fire range.
    float towers_range_by_type[3] = {1.f, 0.71f, 0.5f};
    this->fire_range = towers_damage_by_type[tower_type] * fire_range;
}

// Returns true if a bullet must be fired.
// Fire a bullet taking this object's damage.
// Fires every set amount of frames depending on type.
// Firing is handled by TileMap. This only returns a boolean to shoot.
bool Tower::update(float deltaTime)
{
    if (period_progress >= period)
    {
        period_progress = 0;
        return true;
    }
    period_progress ++;
    return false;
}


void Tower::draw(sf::RenderWindow& window)
{
    // sprite.setPosition(position);
    window.draw(sprite);

}

// Returns the pixel position of the tower
sf::Vector2f Tower::getPosition() const {
    return position;
}


// SFML and movement:
sf::CircleShape sprite;
// std::vector<sf::Vector2f> path;

// {0,1,2} -> {pierce, magic, siege}
int tower_type;
// period between shots in frames.
int period;
int period_progress = 0;

float damage;

sf::Vector2f position;
// float closest_distance_to_target;

void Tower::upgrade()
{
    damage *= 1.2f;
    period = (int)(period * 0.8f);
}


