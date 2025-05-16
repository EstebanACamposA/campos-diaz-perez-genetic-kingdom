#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <memory>

#include <queue>

#include "Projectile.h"

#include "genetics.h"

#include "Tower.h"

class Character; // forward declaration instead of #include.
class TileMap {
public:
    TileMap(int rows, int cols, int tileSize, int wave_lenght, const int (&tile_map)[30][30]);
    void draw(sf::RenderWindow& window);
    void setObstacle(int row, int col, bool isObstacle);
    void update(float deltaTime);
    void addCharacter(std::shared_ptr<Character> character);    // Adds to characters vector.
    void addCharacter(std::shared_ptr<Character> character, int species);   // Adds to species D2 vector.
    void moveCharacterTo(std::shared_ptr<Character> character, sf::Vector2i target);
    std::vector<sf::Vector2i> findPath(sf::Vector2i start, sf::Vector2i end);
    // Game
        // Testing //
    void ShootRandomProjectile();
        // Testing //
    void SpawnNextEnemy();
    void AddWave(std::vector<int> new_wave);
    bool IsCurrentRoundClear();
    bool IsAtLastEnemySpawn();

    // Genetics
    Genetics genetic_manager;

    bool build_buttons;
    bool upgrade_button;
    std::vector<sf::RectangleShape> build_buttons_sprites;
    sf::RectangleShape upgrade_button_sprite;

    // This is used by both the upgrade_button and build_buttons to know what tower was clicked in case there's an action to do.
    // In pixels.
    sf::Vector2i last_succesful_tile_click;
    void clickEvents(sf::Vector2i click_coords);

    // Building tilemap from an int[][]
    void setTiles(const int (&tile_map)[30][30]);



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

    void ShootNearest(sf::Vector2f startPosition, float damage, int tower_type, float fire_range, float projectile_speed, bool power_up_projectile);

    int tower_levels[3];

    int clicked_tower_id;


    int build_button_top_anchors[3];
    int build_button_height;

    //Handle AoR attacks
    void AOEAttack(sf::Vector2f position, bool apply_shock_shell_effect, float damage);

    // Waves
    int wave_lenght;
    std::queue<std::vector<int>> waves_queue;
    int current_round_cleared_enemies;
    // Tells main to Add a new wave. wave == round
    bool current_round_is_cleared;
    int current_round_spawned_enemies;
    // void ClearEnemy(std::vector<int> new_wave)
    void EndWave();

    double mutation_chance;
    int total_mutations;
    
    // SFML Texture
    sf::Texture tower_textures[3];
    sf::Texture enemy_textures[4];

    sf::Texture tile_textures[5];
    std::vector<sf::Sprite> textured_tiles; // This vector is used for drawing the tiles.

    sf::Texture goal_texture;
    sf::Sprite goal_sprite;

    // Money logic
    int money;
    int money_per_species[4];
    // Money Display
    int money_label_timer;
    std::vector<sf::Text> money_labels; // This vector is used for drawing labels displaying money earned for defeating enemies.
    void CreateMoneyLabel(sf::Vector2f cleared_enemy_position, int money_obtained);
    sf::Font font;

    // Info Panel
    void DrawInfoPanel(sf::RenderWindow& window);
    int generations_elapsed;
    sf::Text generations_elapsed_label;
    sf::Text current_round_cleared_enemies_label;
    
    sf::Text orc_best_fitness_label;
    sf::Text ne_best_fitness_label;
    sf::Text harpy_best_fitness_label;
    sf::Text merc_best_fitness_label;
    
    sf::Text tower_levels_label;
    sf::Text mutations_chance_and_total_label;
    // Money label
    sf::Text total_money_label;
    sf::Text upgrade_and_build_costs_label;

    // // Calculates the distance between two characters.
    // float calculateDistanceInPixels()
    
};