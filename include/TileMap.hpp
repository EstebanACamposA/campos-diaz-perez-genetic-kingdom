#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <memory>

#include "Projectile.h"

class Character; // forward declaration instead of #include.
class TileMap {
public:
    TileMap(int rows, int cols, int tileSize);
    void draw(sf::RenderWindow& window);
    void setObstacle(int row, int col, bool isObstacle);
    void update(float deltaTime);
    void addCharacter(std::shared_ptr<Character> character);
    void moveCharacterTo(std::shared_ptr<Character> character, sf::Vector2i target);
    std::vector<sf::Vector2i> findPath(sf::Vector2i start, sf::Vector2i end);
    // Game
        // Testing
    void ShootRandomProjectile();

private:
    std::vector<std::shared_ptr<Character>> characters;
    std::vector<std::shared_ptr<Projectile>> projectiles;
    struct Node {
        int row, col;
        int gCost = 0, hCost = 0, fCost = 0;
        bool isObstacle = false;
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

    int calculateDistance(const Node& a, const Node& b);
    std::vector<Node*> getNeighbors(Node& node);
};