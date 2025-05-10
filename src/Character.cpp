#include "Character.hpp"
#include <cmath>
#include <iostream>

// Base constructor.
Character::Character(sf::Vector2f startPosition, float speed) 
    : position(startPosition), speed(speed) {
    sprite.setRadius(10.f);
    sprite.setFillColor(sf::Color::Blue);
    sprite.setOrigin(10.f, 10.f);
}

// Genetics constructor.
// species {orcs, NE, harpies, mercs} --> {0,1,2,3} 
Character::Character(sf::Vector2f startPosition, Individual individual, int species):
    position(startPosition),
    // Five genes:
    max_health(individual.max_health),
    speed_multiplier(individual.speed_multiplier),
    pierce_armor(individual.pierce_armor),
    magic_armor(individual.magic_armor),
    siege_armor(individual.siege_armor)
{
    // SFML:
    sprite.setRadius(10.f);
    sf::Color enemy_color[4] = {
        sf::Color(200, 225, 170),
        sf::Color(200, 170, 255),
        sf::Color(200, 255, 150),
        sf::Color(180, 190, 150)
    };
    base_color = enemy_color[species];
    sprite.setFillColor(base_color);
    sprite.setOrigin(10.f, 10.f);
    
    // Game logic
    // Species based damage:
    /* guide:                         {orcs, NE, harpies, mercs}
        pierce_damage_multiplier = {1/2, 3/2, 3/2, 1/2}
        magic_damage_multiplier = {5/4, 1/4, 5/4, 5/4}
        siege_damage_multiplier = {3/2, 3/2, 0, 1/2} // harpies are immune to siege attacks.
        so that all species are equally strong, their health is calculated as
        health = 100*species_health_multiplier;
        species_health_multiplier is chosen in the constructor out of {1, 1.5, 1.81, 1.48} (for {orcs, NE, harpies, mercs})
            these are the general damage / spawn rate
                general damage is calculated from the species based damage (above).
                spawn rate is {15,10,7,7}
            then it is divided by the smallest value so that the base mutliplier is 1 (for orcs).
    */

    float species_health_multiplier[] = {1.f, 1.5f, 1.81f, 1.48f};
    max_health *= species_health_multiplier[species];   // This doesn't affect the genetic algorithm. It only makes the species balanced.

    current_health = max_health;
    speed = 50 * speed_multiplier; // 100 is default speed!!! SPEED.

    pierce_damage = 0;
    magic_damage = 0;
    siege_damage = 0;
    damage_taken = 0;

    // Sets damage type damage multipliers.
    // pierce_damage_multiplier = {1/2, 3/2, 3/2, 1/2}
    // magic_damage_multiplier = {5/4, 1/4, 5/4, 5/4}
    // siege_damage_multiplier = {3/2, 3/2, 0, 1/2}
    float species_damage_multipliers[3][4] = {
        {1.f/2, 3.f/2, 3.f/2, 1.f/2},
        {5.f/4, 1.f/4, 5.f/4, 5.f/4},
        {3.f/2, 3.f/2, 0.f, 1.f/2}};
    pierce_damage_multiplier = species_damage_multipliers[0][species];
    magic_damage_multiplier  = species_damage_multipliers[1][species];
    siege_damage_multiplier  = species_damage_multipliers[2][species];

    posion_arrow_effect = false;
    frost_orb_effect = false;
    shock_shell_timer = 0;

    // Active color variables.
    bool active_color_red = false;
    bool active_color_blue = false;
    bool active_color_green = false;


}


// Moves character's postition a unit closer to the next destination in path if available.
std::optional<Individual> Character::update(float deltaTime) {
    // Damage logic make new variable for character


    if (pierce_damage > 0 || magic_damage > 0 || siege_damage > 0 )
    {    
        damage_taken =
        pierce_damage*pierce_damage_multiplier +
        magic_damage*magic_damage_multiplier +
        siege_damage*siege_damage_multiplier;
    
    
        // std::cout << "pierce_damage:" << pierce_damage;
        // std::cout << " magic_damage:" << magic_damage;
        // std::cout << " siege_damage:" << siege_damage;
    
        // std::cout << std::endl;
    
        // std::cout << "pierce_damage_multiplier:" << pierce_damage_multiplier;
        // std::cout << " magic_damage_multiplier:" << magic_damage_multiplier;
        // std::cout << " siege_damage_multiplier:" << siege_damage_multiplier;
    
        // std::cout << std::endl;
    
        pierce_damage = 0.f;
        magic_damage = 0.f;
        siege_damage = 0.f;
        
        std::cout << "current_health before:" << current_health;
        current_health -= damage_taken;
        std::cout << " current_health after:" << current_health << std::endl;

    }
    


    if (current_health <= 0)
    {
        Individual genetic_result = CalculateIndividual();
        return genetic_result; // Returning an Individual type object works even if the return type is std::optional<Individual>.
    }

    // Visual effect. Blinking.
    if (damage_taken > 0)
    {
        // std::cout << "current_health = " << current_health << std::endl;

        if (damage_blink)
        {
            damage_blink = false;
            // Change this to sprite.setColor(sf::Color(255, 255, 255) to remove tint from an image base sprite.
            CalculateColors(-1,0,0); // Deactivate red.
        }
        else
        {
            damage_blink = true;
            CalculateColors(1,0,0); // Activate red.
        }
        damage_taken --;    // The amount substracted from damage_taken tells how much the VFX lasts. damage points / frame.
    }
    else if (damage_blink)
    {
        damage_blink = false;
        CalculateColors(-1,0,0); // Deactivate red.
    }

    // If character hasn't arrived at its last destination.
    if (currentTarget < path.size()) {
        sf::Vector2f direction = path[currentTarget] - position;
        // Magnitude of the direction vector.
        float distance = std::sqrt(direction.x*direction.x + direction.y*direction.y);
        
        // If distance is too short, character has already arrived at currentTarget and targets the next.
        if (distance < speed*deltaTime) {   // if (distance < 5.f)
            currentTarget++;
        } else {
            // Normalizes direction vector.
            direction /= distance;
            
            // Changes the characters postition value, doesn't draw just yet.
            // Inverts direction if shock_effect_timer is on.
            if (shock_shell_timer > 0)
            {
                position -= direction * speed * deltaTime *(float)std::sqrt(shock_shell_timer);    
                shock_shell_timer -= 2;
            }
            else
            {
                position += direction * speed * deltaTime;    
            }
            
        }
    }
    
    // Effect of poison arrow. Deals 60 dps. A level 1 tower deals about 30 dps.
    if (posion_arrow_effect)
    {
        // Active colors
        if (!active_color_green)
        {
            CalculateColors(0,1,0);
        }
        
        current_health --;
    }

    // returns std::nullopt every time the Character is not destroyed.
    return std::nullopt; // This is taken as false inside an if statement.
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

// currently called by main 
void Character::draw(sf::RenderWindow& window) {
    // There could be a VFX related position that follows the actuall position depending on the distance between the two and show this one
    // this would avoid sharp turns.
    sprite.setPosition(position);
    window.draw(sprite);
}

sf::Vector2f Character::getPosition() const {
    return position;
}


Individual Character::CalculateIndividual()
{   
    float completed_path = CalculateCompletedPath();
    float remaining_health = current_health / max_health;

    Individual genetic_result(max_health, speed_multiplier, pierce_armor, magic_armor, siege_armor, completed_path, remaining_health);
    return genetic_result;

}

// Finds what proportion of the Character's path has been traveled. float completed_path -> [0,1]
// Independent of tileSize.
float Character::CalculateCompletedPath()
{
    float completed_distance = 0;
    float remaining_distance = 0;
    for (size_t i = 0; i < path.size() - 1; i++)
    {
        if (i >= currentTarget)
        {
            // Distance is calculated taking into account the possible movements of a Characters.
            // Up, left, down, right.
            remaining_distance += path[i+1].x - path[i].x + path[i+1].y - path[i].y;
        }
        else
        {
            completed_distance += path[i+1].x - path[i].x + path[i+1].y - path[i].y;
        }
    }
    return completed_distance / (completed_distance + remaining_distance);
    
    
}
// Cuts enemies speed by half. Can only be applied once. Is permanent.
void Character::ApplyFrostOrbEffect()
{
    if (!frost_orb_effect)
    {
        CalculateColors(0,0,1);
        frost_orb_effect = true;
        this->speed /= 2;    
    }
}

// r,g,b must belong to {-1,0, 1}
void Character::CalculateColors(int r, int g, int b)
{
    if (r = -1)
    {
        active_color_red = false;
    }
    if (g = -1)
    {
        active_color_green = false;
    }
    if (b = -1)
    {
        active_color_blue = false;
    }
    
    if (r = 1)
    {
        active_color_red = true;
    }
    if (g = 1)
    {
        active_color_green = true;
    }
    if (b = 1)
    {
        active_color_blue = true;
    }
    
    std::cout << "Color input = (" << r << ", " << g << ", " << b << ")" << std::endl;

    sf::Color status_color(
        255 -128*active_color_green - 128*active_color_blue,
        255 -128*active_color_red - 128*active_color_blue,
        255 -128*active_color_red - 128*active_color_green);
    

    sf::Color result_color(base_color.r * status_color.r / 255, base_color.g * status_color.g / 255, base_color.b * status_color.b / 255);
    sprite.setFillColor(result_color);
    std::cout << "result_color = (" << (int) result_color.r << ", " << (int) result_color.g << ", " << (int) result_color.b << ")" << std::endl;


}