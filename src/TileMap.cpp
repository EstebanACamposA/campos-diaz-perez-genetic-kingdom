#include "TileMap.hpp"
#include "Character.hpp"
TileMap::TileMap(int rows, int cols, int tileSize) 
    : rows(rows), cols(cols), tileSize(tileSize) {
    // grid is a 2D vector of nodes.
    // Creates a grid of uninitialized nodes
    grid.resize(rows, std::vector<Node>(cols));
    // Assigns the nodes
    // node struct attributes {int row, col; int gCost = 0, hCost = 0, fCost = 0; bool isObstacle = false; Node* parent}
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            grid[y][x] = {y, x, 0, 0, 0, false, nullptr};

            // Creates white sf::RectangleShape tiles and stores them in the tiles (1D) vector. This 1D vector is used similar to a matrix.
            sf::RectangleShape tile(sf::Vector2f(tileSize - 1, tileSize - 1));
            tile.setPosition(x * tileSize, y * tileSize);
            tile.setFillColor(sf::Color::White);
            tile.setOutlineColor(sf::Color::Black);
            tile.setOutlineThickness(1.f);
            tiles.push_back(tile);
        }
    }

    // Matches the number of rows of enemy_species with the number of species, 4.
    for (size_t i = 0; i < 4; i++)
    {
        std::vector<std::shared_ptr<Character>> new_row;
        enemy_species.push_back(new_row);
    }
    
}

// Acceses grid and tiles at (row,col) and assigns isObstacle. White and free or Red and obstacle.
void TileMap::setObstacle(int row, int col, bool isObstacle) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        grid[row][col].isObstacle = isObstacle;
        tiles[row * cols + col].setFillColor(isObstacle ? sf::Color::Red : sf::Color::White);
    }
}

std::vector<sf::Vector2i> TileMap::findPath(sf::Vector2i start, sf::Vector2i end) {
    std::vector<sf::Vector2i> path;
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    std::unordered_map<Node*, bool> closedSet;

    // Reset nodes (importante para múltiples búsquedas)
    for (auto& row : grid) {
        for (auto& node : row) {
            node.gCost = node.hCost = node.fCost = 0;
            node.parent = nullptr;
        }
    }

    // x, y must belong to the grid.
    Node* startNode = &grid[start.y][start.x];
    Node* endNode = &grid[end.y][end.x];

    startNode->gCost = 0;
    startNode->hCost = calculateDistance(*startNode, *endNode);
    startNode->calculateFCost();

    // The priority_queue adds elements in order. Nodes are compared by fCost.
    openSet.push(startNode);

    while (!openSet.empty()) {
        // current is the best node in the priority_queue.
        Node* current = openSet.top();
        openSet.pop();

        // If the current node is the destination.
        if (current == endNode) {
            // Builds the path starting with the endNode.
            // Adds a node's (x,y) coords as a Vector2i to the path and does the same to it's parent until parent is null 
            while (current != nullptr) {
                path.emplace_back(current->col, current->row);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Current node has been visited.
        closedSet[current] = true;
        // Each of the usually 4 adjacent nodes.
        for (Node* neighbor : getNeighbors(*current)) {
            // If the node is an obstacle or has already been analyzed by this for loop.
            if (neighbor->isObstacle || closedSet[neighbor]) continue;

            // This may not work with more directions?
            int tentativeGCost = current->gCost + calculateDistance(*current, *neighbor);
            // If a better parent has been found for the neighbor.
            // If true, adds the neighbor to openSet. Will eventually add the target node to openSet.
            if (tentativeGCost < neighbor->gCost || !neighbor->parent) {
                neighbor->parent = current;
                neighbor->gCost = tentativeGCost;
                neighbor->hCost = calculateDistance(*neighbor, *endNode);
                neighbor->calculateFCost();

                openSet.push(neighbor);
            }
        }
    }

    return path; // Camino vacío si no hay solución
}

int TileMap::calculateDistance(const Node& a, const Node& b) {
    // Distancia Manhattan (para 4 direcciones)
    return abs(a.row - b.row) + abs(a.col - b.col);
}

// Gets from grid the four adjacent nodes to the given one: up, right, down, left.
// If a node is not available for being outside of bounds, it is not returned. Doesn't check for obstacles.
std::vector<TileMap::Node*> TileMap::getNeighbors(Node& node) {
    std::vector<Node*> neighbors;
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    for (auto& dir : directions) {
        int newRow = node.row + dir[0];
        int newCol = node.col + dir[1];

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            neighbors.push_back(&grid[newRow][newCol]);
        }
    }

    return neighbors;
}

// Draws all the tiles, characters, and projectiles.
void TileMap::draw(sf::RenderWindow& window) {
    // Tiles
    for (const auto& tile : tiles) {
        window.draw(tile);
    }
    // // Characters.
    // for (size_t i = 0; i < characters.size(); i++)
    // {
    //     (*characters[i]).draw(window);
    // }
    // Characters of species 2D vector
    for (size_t i = 0; i < enemy_species.size(); i++)
    {
        for (size_t j = 0; j < enemy_species[i].size(); j++)
        {
            (*enemy_species[i][j]).draw(window);
        }
    }
    // Projectiles.
    for (size_t i = 0; i < projectiles.size(); i++)
    {
        (*projectiles[i]).draw(window);
    }
}

// Testing function.
// Add a Character to the characters vector. No species.
void TileMap::addCharacter(std::shared_ptr<Character> character) {
    characters.push_back(character);
}
// Add a Character to the enemy_species D2 vector. A specific species id is required.
void TileMap::addCharacter(std::shared_ptr<Character> character, int species) {
    enemy_species[species].push_back(character);

    sf::Vector2i target(rows-1, cols-1);    // testing behaviour.
    moveCharacterTo(character, target);
}



// Give a new path to a Character.
// Characters are made to automatically move along a path if they have one.
void TileMap::moveCharacterTo(std::shared_ptr<Character> character, sf::Vector2i target) {
    sf::Vector2i start = {
        static_cast<int>(character->getPosition().x / tileSize),
        static_cast<int>(character->getPosition().y / tileSize)
    };
    
    auto path = findPath(start, target);
    // when calling findPath the grid is not modified, only used. VERIFY THIS!!!
    character->setPath(path, tileSize);
}

// Runs Character.update(deltatime) on all Characters in this->characters and on all projectiles.
// (Character updates are handled by TileMap).
void TileMap::update(float deltaTime) {
    // Uses iterators to erase at the same time it iterates through the vector.

    // Iterates through the projectiles vector to update positions and remove if marked for deletion.
    for (auto it = projectiles.begin(); it != projectiles.end(); )
    {
        if ((*it)->update(deltaTime))   // If the current Projectile is marked for deletion.
        {
            it = projectiles.erase(it); /* This erases the current Projectile from the list and returns a new iterator.
            This is where the iterators helps in not getting errors when iterating through the list.*/
        } else
        {
            ++it; // goes to the next position only if nothing was erased. This is part of this specific pattern to 
        }
    }
//////////////////////////////////////////////////
    // // Uses characters (that holds test Characters with no related species) instead of enemy_species (new D2 vector for storing Characters).
    // for (auto it = characters.begin(); it != characters.end(); )
    // {
    //     if ((*it)->update(deltaTime))   // The return type of Character::update is std::optional<Individual>. It works as true or false.
    //     {
    //         it = characters.erase(it);
    //     } else
    //     {
    //         ++it;
    //     }
    // }
//////////////////////////////////////////////////
    
// For each species in enemy_species, for each enemy, updates all positions
// Removes characters marked for deletion and sends them to the genetic_manager.
    // std::cout << std::endl;
    for (size_t i = 0; i < enemy_species.size(); i++)
    {
        // std::cout << "enters for enemy_species.size" << std::endl;
        for (auto it = enemy_species[i].begin(); it != enemy_species[i].end(); )
        {
            // std::cout << "enters for enemy_species[i].size" << std::endl;
            if ((*it)->update(deltaTime))   // The return type of Character::update is std::optional<Individual>. It works as true or false.
            {
                Individual individual_from_character = (*it)->CalculateIndividual();
                genetic_manager.AddIndividual(i, individual_from_character);

                std::cout <<  "before enemy_species[" << i << "].size() = " << enemy_species[i].size() << std::endl;
                it = enemy_species[i].erase(it);
                std::cout << "Erased a Character of species: " << i << std::endl;
                std::cout <<  "after enemy_species[" << i << "].size() = " << enemy_species[i].size() << std::endl;

            } else
            {
                ++it;
            }
        }
    }

}




// // Testing function.
// // Adds a projectile to projectiles with a random target Character from characters.
// void TileMap::ShootRandomProjectile()
// {
//     std::cout << "Enters ShootRandomProjectile()" << std::endl;
//     // tiles[rows * cols /2 + cols/2]
//     sf::Vector2f projectile_start_position((float)rows*tileSize/2, (float)cols*tileSize/2);
//     std::cout << "CHECKPOINT 1" << std::endl;
//     std::shared_ptr<Character> target_character = characters[characters.size()/2];
//     std::cout << "CHECKPOINT 2" << std::endl;
//     // Projectile projectile(projectile_start_position, target_character, 200.f);
//     std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(projectile_start_position, target_character, 200.f);
//     std::cout << "CHECKPOINT 3" << std::endl;
//     projectiles.push_back(projectile);
//     std::cout << "CHECKPOINT 4" << std::endl;

// }

// Testing function. New.
// Adds a projectile to projectiles with a random target Character from vector enemy_species[0].
void TileMap::ShootRandomProjectile()
{
    std::cout << "Enters ShootRandomProjectile()" << std::endl;
    if (enemy_species.size() == 0)
    {
        std::cout <<  "enemy_species.size() == 0. Exits TileMap::ShootRandomProjectile()" << std::endl;
        return;
    }
    if (enemy_species[0].size() == 0)
    {
        std::cout <<  "enemy_species[0].size() == 0. Exits TileMap::ShootRandomProjectile()" << std::endl;
        return;
    }

    // tiles[rows * cols /2 + cols/2]
    sf::Vector2f projectile_start_position((float)rows*tileSize/2, (float)cols*tileSize/2);
    // std::cout << "CHECKPOINT 1" << std::endl;
    std::shared_ptr<Character> target_character = enemy_species[0][enemy_species[0].size()/2];
    // std::cout << "CHECKPOINT 2" << std::endl;
    // Projectile projectile(projectile_start_position, target_character, 200.f);
    std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(projectile_start_position, target_character, 200.f);
    // std::cout << "CHECKPOINT 3" << std::endl;
    projectiles.push_back(projectile);
    // std::cout << "CHECKPOINT 4" << std::endl;

}





