#include "TileMap.hpp"

TileMap::TileMap(int rows, int cols, int tileSize) 
    : rows(rows), cols(cols), tileSize(tileSize) {
    grid.resize(rows, std::vector<Node>(cols));

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            grid[y][x] = {y, x, 0, 0, 0, false, nullptr};

            sf::RectangleShape tile(sf::Vector2f(tileSize - 1, tileSize - 1));
            tile.setPosition(x * tileSize, y * tileSize);
            tile.setFillColor(sf::Color::White);
            tile.setOutlineColor(sf::Color::Black);
            tile.setOutlineThickness(1.f);
            tiles.push_back(tile);
        }
    }
}

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

    Node* startNode = &grid[start.y][start.x];
    Node* endNode = &grid[end.y][end.x];

    startNode->gCost = 0;
    startNode->hCost = calculateDistance(*startNode, *endNode);
    startNode->calculateFCost();

    openSet.push(startNode);

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current == endNode) {
            while (current != nullptr) {
                path.emplace_back(current->col, current->row);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet[current] = true;

        for (Node* neighbor : getNeighbors(*current)) {
            if (neighbor->isObstacle || closedSet[neighbor]) continue;

            int tentativeGCost = current->gCost + calculateDistance(*current, *neighbor);
            
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