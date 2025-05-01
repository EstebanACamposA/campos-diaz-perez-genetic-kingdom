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

void TileMap::draw(sf::RenderWindow& window) {
    for (const auto& tile : tiles) {
        window.draw(tile);
    }
}
void TileMap::addCharacter(std::shared_ptr<Character> character) {
    characters.push_back(character);
}

// Characters are made to automatically move along a path if they one.
void TileMap::moveCharacterTo(std::shared_ptr<Character> character, sf::Vector2i target) {
    sf::Vector2i start = {
        static_cast<int>(character->getPosition().x / tileSize),
        static_cast<int>(character->getPosition().y / tileSize)
    };
    
    auto path = findPath(start, target);
    // when calling findPath the grid is not modified, only used. VERIFY THIS!!!
    character->setPath(path, tileSize);
}

void TileMap::update(float deltaTime) {
    for (auto& character : characters) {
        character->update(deltaTime);
    }
}