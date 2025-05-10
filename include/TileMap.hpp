#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <memory>

#include "Projectile.h"

#include "genetics.h"

#include "Tower.h"

class Character; // forward declaration instead of #include.
class TileMap {
public:
    TileMap(int rows, int cols, int tileSize);
    void draw(sf::RenderWindow& window);
    void setObstacle(int row, int col, bool isObstacle);
    void update(float deltaTime);
    void addCharacter(std::shared_ptr<Character> character);    // Adds to characters vector.
    void addCharacter(std::shared_ptr<Character> character, int species);   // Adds to species D2 vector.
    void moveCharacterTo(std::shared_ptr<Character> character, sf::Vector2i target);
    std::vector<sf::Vector2i> findPath(sf::Vector2i start, sf::Vector2i end);
    // Game
        // Testing
    void ShootRandomProjectile();

    // Genetics
    Genetics genetic_manager;

    bool build_buttons;
    bool upgrade_button;
    std::vector<sf::RectangleShape> build_buttons_sprites;
    sf::RectangleShape upgrade_button_sprite;

    sf::Vector2i last_succesful_tile_click;
    void clickEvents(sf::Vector2i click_coords);


private:
    std::vector<std::shared_ptr<Character>> characters;
    // A D2 vector of std::shared_ptr<Character>
    std::vector<std::vector<std::shared_ptr<Character>>> enemy_species;
    
    std::vector<std::shared_ptr<Projectile>> projectiles;

    //A vector of towers. A row for each tower type (3 rows/types)
    std::vector<std::shared_ptr<Tower>> towers;

    
    struct Node {
        int row, col;
        int gCost = 0, hCost = 0, fCost = 0;
        bool isObstacle = false;
        bool isBuildable = false;
        Node* parent = nullptr;

        void calculateFCost() { fCost = gCost + hCost; }
    };

    // Functor para comparar nodos
    struct CompareNode {
        bool operator()(const Node* a, const Node* b) const {
            return a->fCost > b->fCost; // Para orden de menor a mayor
        }
    };

    std::vector<std::vector<Node>> grid;
    std::vector<sf::RectangleShape> tiles;
    int rows, cols, tileSize;

    // Calculates 'Manhattan distance between two nodes.'
    int calculateDistance(const Node& a, const Node& b);
    std::vector<Node*> getNeighbors(Node& node);

    void ShootNearest(sf::Vector2f startPosition, float damage, int tower_type);

    int pierce_tower_level;
    int magic_tower_level;
    int siege_tower_level;

    int clicked_tower_id;

    // // Calculates the distance between two characters.
    // float calculateDistanceInPixels()
};