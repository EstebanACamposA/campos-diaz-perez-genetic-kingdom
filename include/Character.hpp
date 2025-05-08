#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include <optional>

// Genetics
#include "genetics.h"

class Character {
public:
    // Basics and movement:
    Character(sf::Vector2f startPosition, float speed = 100.f); // 100.f is default speed. 
    std::optional<Individual> update(float deltaTime);
    void setPath(const std::vector<sf::Vector2i>& newPath, float tileSize);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    // Game variables:
    // Damage variables are increased by projectiles on impact and reduce health in the Character's Update() function.
    float pierce_damage;
    float magic_damage;
    float siege_damage;
    // Constructor for genetic logic.
    Character(sf::Vector2f startPosition, Individual genes, int species);

    Individual CalculateIndividual();
    float CalculateCompletedPath();

private:
    // SFML and movement:
    sf::CircleShape sprite;
    std::vector<sf::Vector2f> path;
    float speed;
    size_t currentTarget = 0;
    sf::Vector2f position;

    // Game variables:
    // genes guide:
        // float max_health = 100;
        // float speed_multiplier = 1;
        // float pierce_armor = 1;
        // float magic_armor = 1;
        // float siege_armor = 1;
    float max_health;
    float speed_multiplier;
    float pierce_armor;
    float magic_armor;
    float siege_armor;
    // Lifetime modified variables.
    float current_health;
    // Species based damage:
    /* guide:                         {orcs, NE, harpies, mercs}
        pierce_damage_multiplier = {1/2, 3/2, 3/2, 1/2}
        magic_damage_multiplier = {5/4, 1/4, 5/4, 5/4}
        siege_damage_multiplier = {3/2, 3/2, 0, 1/2} // harpies are immune to siege attacks.
        so that all species are equally strong, their health is calculated as
        health = max_health*species_health_multiplier;
        species_health_multiplier is chosen in the constructor out of {1, 1.5, 1.81, 1.48} (for {orcs, NE, harpies, mercs})
            these are the general damage / spawn rate
                general damage is calculated from the species based damage (above).
                spawn rate is {15,10,7,7}
            then it is divided by the smallest value so that the base mutliplier is 1 (for orcs).
    */
    float pierce_damage_multiplier;
    float magic_damage_multiplier;
    // May need to make these values public so that bullets with special abilities know when not to affect immune units (harpies to siege).
    float siege_damage_multiplier;
    // Visual effect variable
    float damage_taken;
    bool damage_blink;


};