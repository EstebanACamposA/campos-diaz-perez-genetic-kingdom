#include "TileMap.hpp"
#include "Character.hpp"

#include <cmath>

TileMap::TileMap(int rows, int cols, int tileSize, int wave_lenght, const int (&tile_map)[30][30]) 
    : rows(rows), cols(cols), tileSize(tileSize), wave_lenght(wave_lenght) {
    // grid is a 2D vector of nodes.
    // Creates a grid of uninitialized nodes
    grid.resize(rows, std::vector<Node>(cols));

    // SFML Texture
    // sf::Texture tile_textures[3];
    if (!tile_textures[0].loadFromFile("W3_sprites/Map/Nightelf-inventory-slotfiller.png")) //Walkable Unbuildable
    {
        std::cout << "Error at loading tile texture 0" << std::endl;
    }
    if (!tile_textures[1].loadFromFile("W3_sprites/Map/Undead-inventory-slotfiller.png"))   //Unwalkable Buildable
    {
        std::cout << "Error at loading tile texture 1" << std::endl;
    }
    if (!tile_textures[2].loadFromFile("W3_sprites/Map/Orc-inventory-slotfiller.png"))  //Walkable Buildable
    {
        std::cout << "Error at loading tile texture 2" << std::endl;
    }


    // Creates a sprite for a tile before the double for cycle.
    // In the double for, adds a copy of the sprite for each tile in the map to std::vector<sf::Sprite> textured_tiles. Changes the position of each one
    // Tiles should then be set to values according to the tile_map through setTiles(const int (&tile_map)[30][30]).
    // sf::Sprite textured_tile;
    // textured_tile.setTexture(tile_textures[0]);
    // // Scaling assuming squared tiles.
    // float scale = tileSize / tile_textures[0].getSize().x;
    // textured_tile.setScale(scale, scale);
    
    // textured_tile.setOrigin(tile_textures[0].getSize().x / 2, tile_textures[0].getSize().y / 2);

    // Assigns the nodes
    // node struct attributes {int row, col; int gCost = 0, hCost = 0, fCost = 0; bool isObstacle = false; Node* parent}
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            grid[x][y] = {x, y, 0, 0, 0, false, false, nullptr};

            /// Testing visuals ///
            // Creates white sf::RectangleShape tiles and stores them in the tiles (1D) vector. This 1D vector is used similar to a matrix.
            // sf::RectangleShape tile(sf::Vector2f(tileSize - 1, tileSize - 1));
            // tile.setPosition(x * tileSize, y * tileSize);
            // tile.setFillColor(sf::Color::White);
            // tile.setOutlineColor(sf::Color::Black);
            // tile.setOutlineThickness(1.f);
            // tiles.push_back(tile);
            /// Testing visuals ///



            if (tile_map[x][y] == 0)
            {
                grid[x][y].isObstacle = false;
                grid[x][y].isBuildable = false;

                
                sf::Sprite textured_tile;
                textured_tile.setTexture(tile_textures[0]);
                // Scaling assuming squared tiles.
                float scale = (float)tileSize / tile_textures[0].getSize().x;
                textured_tile.setScale(scale, scale);
                
                // textured_tile.setOrigin(tile_textures[0].getSize().x / 2, tile_textures[0].getSize().y / 2);
                // textured_tile.setPosition(x * tileSize, y * tileSize);
                textured_tile.setPosition(y * tileSize, x * tileSize);
                textured_tiles.push_back(textured_tile);
            
            }

            if (tile_map[x][y] == 1)
            {
                grid[x][y].isObstacle = true;
                grid[x][y].isBuildable = true;

                
                sf::Sprite textured_tile;
                textured_tile.setTexture(tile_textures[1]);
                // Scaling assuming squared tiles.
                float scale = (float)tileSize / tile_textures[1].getSize().x;
                textured_tile.setScale(scale, scale);
                
                // textured_tile.setOrigin(tile_textures[1].getSize().x / 2, tile_textures[0].getSize().y / 2);
                textured_tile.setPosition(y * tileSize, x * tileSize);
                textured_tiles.push_back(textured_tile);
            
            }

            if (tile_map[x][y] == 2)
            {
                grid[x][y].isObstacle = false;
                grid[x][y].isBuildable = true;

                
                sf::Sprite textured_tile;
                textured_tile.setTexture(tile_textures[2]);
                // Scaling assuming squared tiles.
                float scale = (float)tileSize / tile_textures[2].getSize().x;
                textured_tile.setScale(scale, scale);
                
                // textured_tile.setOrigin(tile_textures[2].getSize().x / 2, tile_textures[0].getSize().y / 2);
                textured_tile.setPosition(y * tileSize, x * tileSize);
                textured_tiles.push_back(textured_tile);
            
            }

        }
        
    }

    // Matches the number of rows of enemy_species with the number of species, 4.
    for (size_t i = 0; i < 4; i++)
    {
        std::vector<std::shared_ptr<Character>> new_row;
        enemy_species.push_back(new_row);
    }

    // Set initial tower levels
    
    // pierce_tower_level = 0;
    // magic_tower_level = 0;
    // siege_tower_level = 0;
    tower_levels[0] = 0;
    tower_levels[1] = 0;
    tower_levels[2] = 0;

    // No selected tower at the start;
    clicked_tower_id = -1;
    build_buttons = false;
    upgrade_button = false;
    
    // Right side panel graphics.
    int left_panel_parts_y = 6; // How many parts the height is visually divided into to fit buttons and the info panel. Buttons take 1 part; info panel, many.
    int screen_height = this->cols * this->tileSize;

    build_button_top_anchors[0] = {screen_height*1/(left_panel_parts_y + 1)};
    build_button_top_anchors[1] = {screen_height*2/(left_panel_parts_y + 1)};
    build_button_top_anchors[2] = {screen_height*3/(left_panel_parts_y + 1)};
    build_button_height = screen_height/(left_panel_parts_y + 1)/2;

    /*
    button 1:
        y: [,]
        x: [,]
    button 2:
        y: [,]
        x: [,]
    button 3:
        y: [,]
        x: [,]    
    */
    // if (click_coords.x >= 31*tileSize && click_coords.x <= 37*tileSize) // x interval of buttons.
    // if (click_coords.y >= build_button_top_anchors[0] && click_coords.y <= build_button_top_anchors[0] + build_button_height)
    // button 1:
    sf::RectangleShape button(sf::Vector2f(7*tileSize - 1, build_button_height - 1));
    button.setPosition(31 * tileSize, build_button_top_anchors[0]);
    button.setFillColor(sf::Color(225, 225, 255));  // Light Dull Blue.
    button.setOutlineColor(sf::Color::Black);
    button.setOutlineThickness(1.5f);
    build_buttons_sprites.push_back(button);
    // button 2:
    // sf::RectangleShape button(sf::Vector2f(7*tileSize - 1, 7*tileSize - 1));
    button.setPosition(31 * tileSize, build_button_top_anchors[1]);
    button.setFillColor(sf::Color(255, 225, 240));  // Light Dull Magenta.
    button.setOutlineColor(sf::Color::Black);
    button.setOutlineThickness(1.5f);
    build_buttons_sprites.push_back(button);
    // button 3:
    // sf::RectangleShape button(sf::Vector2f(7*tileSize - 1, 7*tileSize - 1));
    button.setPosition(31 * tileSize, build_button_top_anchors[2]);
    button.setFillColor(sf::Color(255, 240, 225));  // Light Dull Orange.
    button.setOutlineColor(sf::Color::Black);
    button.setOutlineThickness(1.5f);
    build_buttons_sprites.push_back(button);    
    
    // upgrade button:
    // sf::RectangleShape button(sf::Vector2f(7*tileSize - 1, 7*tileSize - 1));
    button.setPosition(31 * tileSize, build_button_top_anchors[1]);
    button.setFillColor(sf::Color(225, 225, 225));
    button.setOutlineColor(sf::Color::Black);
    button.setOutlineThickness(1.5f);
    upgrade_button_sprite = button;

    // Wave
    current_round_cleared_enemies = 0;
    current_round_is_cleared = false;
    current_round_spawned_enemies = 0;

    mutation_chance = 0.5;   // Enemies have 50% chance to mutate all stats.
    std::cout << "after mutation_chance = 0.5; mutation_chance = " << mutation_chance << std::endl;
    std::cout << "after mutation_chance = 0.5; mutation_chance = " << mutation_chance << std::endl;
    std::cout << "after mutation_chance = 0.5; mutation_chance = " << mutation_chance << std::endl;
    total_mutations = 0;

    // SFML Texture
    // sf::Texture tower_textures[3];
    if (!tower_textures[0].loadFromFile("W3_sprites/Towers/BTNGuardTower.png"))
    {
        std::cout << "Error at loading tower texture 0" << std::endl;
    }
    if (!tower_textures[1].loadFromFile("W3_sprites/Towers/BTNHumanArcaneTower.png"))
    {
        std::cout << "Error at loading tower texture 1" << std::endl;
    }
    if (!tower_textures[2].loadFromFile("W3_sprites/Towers/BTNCannonTower.png"))
    {
        std::cout << "Error at loading tower texture 2" << std::endl;
    }
    // sf::Texture enemy_textures[3];
    if (!enemy_textures[0].loadFromFile("W3_sprites/Units/BTNGrunt.png"))
    {
        std::cout << "Error at loading enemy texture 0" << std::endl;
    }
    if (!enemy_textures[1].loadFromFile("W3_sprites/Units/BTNArcher.png"))
    {
        std::cout << "Error at loading enemy texture 1" << std::endl;
    }
    if (!enemy_textures[2].loadFromFile("W3_sprites/Units/BTNHarpy.png"))
    {
        std::cout << "Error at loading enemy texture 2" << std::endl;
    }
    if (!enemy_textures[3].loadFromFile("W3_sprites/Units/BTNBandit.png"))
    {
        std::cout << "Error at loading enemy texture 3" << std::endl;
    }



}


// Testing function
// Acceses grid and tiles at (row,col) and assigns isObstacle. White and free or Red and obstacle.
// Makes all obstacles buildable.
void TileMap::setObstacle(int row, int col, bool isObstacle) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        grid[row][col].isObstacle = isObstacle;
        grid[row][col].isBuildable = true;
        tiles[row * cols + col].setFillColor(isObstacle ? sf::Color::Red : sf::Color::White);
    }
}

// Sets both the visuals and (walkable, buidable) properties of the tiles.
// Map is fixed to be 30x30.
void TileMap::setTiles(const int (&tile_map)[30][30])
{
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 30; ++j) {
            if (tile_map[i][j] == 0) {
                grid[i][j].isObstacle = false;
                grid[i][j].isBuildable = false;
                textured_tiles[i * cols + j].setTexture(tile_textures[0]);
                // std::cout << "setTiles of type 0 at (i,j) = (" << i << ", " << j << ")" << std::endl;
            }
            if (tile_map[i][j] == 1) {
                grid[i][j].isObstacle = true;
                grid[i][j].isBuildable = true;
                textured_tiles[i * cols + j].setTexture(tile_textures[1]);
                // std::cout << "setTiles of type 1 at (i,j) = (" << i << ", " << j << ")" << std::endl;
            }
            if (tile_map[i][j] == 2) {
                grid[i][j].isObstacle = false;
                grid[i][j].isBuildable = true;
                textured_tiles[i * cols + j].setTexture(tile_textures[2]);
                // std::cout << "setTiles of type 2 at (i,j) = (" << i << ", " << j << ")" << std::endl;
            }
        }
        
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

// Draws all the tiles, characters, projectiles, towers, and buttons, respectively.
void TileMap::draw(sf::RenderWindow& window) {
    /// Testing visuals ///
    // Tiles
    // for (const auto& tile : tiles) {
    //     window.draw(tile);
    // }
    /// Testing visuals ///

    // Textured Tiles
    for (size_t i = 0; i < textured_tiles.size(); i++)
    {
        window.draw(textured_tiles[i]);

        // // Check if textured_tiles[i] has a texture.
        // if (textured_tiles[i].getTexture() == nullptr)
        // {
        //     std::cout << "Sprite has no texture!" << std::endl;
        // }
        // sf::Vector2u texSize = textured_tiles[i].getTexture()->getSize();
        // std::cout << "Texture size (tile): " << texSize.x << "x" << texSize.y << std::endl;

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
    // Towers.
    for (size_t i = 0; i < towers.size(); i++)
    {
        towers[i]->draw(window);
    }
    // buttons.
    if (build_buttons)
    {
        for (size_t i = 0; i < build_buttons_sprites.size(); i++)
        {
            window.draw(build_buttons_sprites[i]);
        }
    }
    if (upgrade_button)
    {
        window.draw(upgrade_button_sprite);
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
// updates towers.
// (Character updates are handled by TileMap).
// Make update return true when a wave is finished.
void TileMap::update(float deltaTime) {
    // Uses iterators to erase at the same time it iterates through the vector.

    // Iterates through the projectiles vector to update positions and remove if marked for deletion.
    for (auto it = projectiles.begin(); it != projectiles.end(); )
    {
        if ((*it)->update(deltaTime))   // If the current Projectile is marked for deletion.
        {
            if ((*it)->explosive_damage)
            {

                AOEAttack((*it)->getPosition(), (*it)->apply_shock_shell_effect, (*it)->getDamage());
            }
            

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
                // Sends genetic data to genetic manager, increases clearedd enemy count for current wave and erases the enemy from the game.
                Individual individual_from_character = (*it)->CalculateIndividual();
                genetic_manager.AddIndividual(i, individual_from_character);
                current_round_cleared_enemies ++;

                std::cout <<  "before enemy_species[" << i << "].size() = " << enemy_species[i].size() << std::endl;
                it = enemy_species[i].erase(it);
                std::cout << "Erased a Character of species: " << i << std::endl;
                std::cout <<  "after enemy_species[" << i << "].size() = " << enemy_species[i].size() << std::endl;

                // When all the enemies of the current wave have been cleared.
                if (current_round_cleared_enemies >= wave_lenght)
                {
                    EndWave();
                }
                

            } else
            {
                ++it;
            }
        }
    }

    for (size_t i = 0; i < towers.size(); i++)
    {
        if (towers[i]->update(deltaTime))
        {
            float damage = towers[i]->damage;
            int tower_type = towers[i]->tower_type;
            float fire_range = towers[i]->fire_range;
            sf::Vector2f start_position = towers[i]->getPosition();
            bool power_up_projectile = Genetics::RandomBool(0.25);
            float projectile_speed = towers[i]->projectile_speed;
            ShootNearest(start_position, damage, tower_type, fire_range, projectile_speed, power_up_projectile);   // ShootNearest(start_position, damage, tower_type, FIRERANGE AQUI AAAA);
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
    // std::cout << "Enters ShootRandomProjectile()" << std::endl;
    if (enemy_species.size() == 0)
    {
        // std::cout <<  "enemy_species.size() == 0. Exits TileMap::ShootRandomProjectile()" << std::endl;
        return;
    }
    if (enemy_species[0].size() == 0)
    {
        // std::cout <<  "enemy_species[0].size() == 0. Exits TileMap::ShootRandomProjectile()" << std::endl;
        return;
    }

    // tiles[rows * cols /2 + cols/2]
    sf::Vector2f projectile_start_position((float)rows*tileSize/2, (float)cols*tileSize/2);
    // std::cout << "CHECKPOINT 1" << std::endl;
    std::shared_ptr<Character> target_character = enemy_species[0][enemy_species[0].size()/2];
    // std::cout << "CHECKPOINT 2" << std::endl;
    // Projectile projectile(projectile_start_position, target_character, 200.f);
    std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(projectile_start_position, target_character, 200.f, 50.f);
    // std::cout << "CHECKPOINT 3" << std::endl;
    projectiles.push_back(projectile);
    // std::cout << "CHECKPOINT 4" << std::endl;

}







//// OLD CLICK EVENTS FOR NON PIXEL COORDINATES (index coordinates). ////

// void TileMap::clickEvents(sf::Vector2i click_coords)
// {
//     std::cout << "click_coords.x/tileSize = " << click_coords.x/tileSize << " click_coords.y/tileSize = " << click_coords.y/tileSize << std::endl;
//     std::cout << "build_buttons = " << build_buttons << "upgrade_button = " << upgrade_button << std::endl;
    

//     // checks current state of displayed buttons

//     // No menu shown.
//     if (!build_buttons && !upgrade_button)
//     {
//         // AIAIAIAIAIAIAIAIA // Need to make upgrade button pop up.
//         if (grid[click_coords.y][click_coords.x].isBuildable)
//         {
//             std::cout << "Obstacle clicked" << std::endl;
//             // Do something with the obstacle
//             last_succesful_tile_click = click_coords;

//             sf::Vector2f position_of_possibly_clicked_tower((last_succesful_tile_click.x + 0.5)*tileSize, (last_succesful_tile_click.y + 0.5)*tileSize);
//             // Search possibly clicked tower's id in towers:
//             clicked_tower_id = -1;
//             for (size_t i = 0; i < towers.size(); i++)
//             {
//                 // If it's close enough
//                 if (towers[i]->getPosition().x - position_of_possibly_clicked_tower.x < 0.5*tileSize && towers[i]->getPosition().y - position_of_possibly_clicked_tower.y < 0.5*tileSize)
//                 {
//                     clicked_tower_id = i;
//                     upgrade_button = true;
//                     return;
//                 }
//             }
//             // Clicked an empty buildable tile.
//             build_buttons = true;
//             return;
//         }
//         else
//         {
//             std::cout << "Empty unbuildable tile clicked." << std::endl;
//             // Do nothing with the empty tile
//         }
     
//     }
//     // If build buttons are displayed
//     // If the build menu is open will result in the menu closing and clicks on the buttons build a tower where previously clicked.
//     if (build_buttons && !upgrade_button)
//     {
//         /*
//         button 1:
//             y: [1,7]
//             x: [31,37]
//         button 2:
//             y: [9,15]
//             x: [31,37]
//         button 3:
//             y: [16,22]
//             x: [31,37]    
//         */
//         if (click_coords.x >= 31 && click_coords.x <= 37)
//         {
//             if (click_coords.y >= 1 && click_coords.y <= 7)
//             {
//                 std::cout << "Button 1 clicked" << std::endl;
//                 // Do something with button 1
//                 build_buttons = false;
                
//                 // Gets a Vector2f to the center of the tile indicated by click_coords. Multiplies by tile size to get the pixel position.
//                 sf::Vector2f startPosition((last_succesful_tile_click.x + 0.5)*tileSize, (last_succesful_tile_click.y + 0.5)*tileSize);
//                 std::shared_ptr<Tower> tower = std::make_shared<Tower>(startPosition, 0, pierce_tower_level, tileSize);
//                 towers.push_back(tower);
//                 return;

//             }
//             else if (click_coords.y >= 9 && click_coords.y <= 15)
//             {
//                 std::cout << "Button 2 clicked" << std::endl;
//                 build_buttons = false;
                
//                 // Gets a Vector2f to the center of the tile indicated by click_coords. Multiplies by tile size to get the pixel position.
//                 sf::Vector2f startPosition((last_succesful_tile_click.x + 0.5)*tileSize, (last_succesful_tile_click.y + 0.5)*tileSize);
//                 std::shared_ptr<Tower> tower = std::make_shared<Tower>(startPosition, 1, magic_tower_level, tileSize);
//                 towers.push_back(tower);
//                 return;
//             }
//             else if (click_coords.y >= 16 && click_coords.y <= 22)
//             {
//                 std::cout << "Button 3 clicked" << std::endl;
//                 build_buttons = false;
                
//                 // Gets a Vector2f to the center of the tile indicated by click_coords. Multiplies by tile size to get the pixel position.
//                 sf::Vector2f startPosition((last_succesful_tile_click.x + 0.5)*tileSize, (last_succesful_tile_click.y + 0.5)*tileSize);
//                 std::shared_ptr<Tower> tower = std::make_shared<Tower>(startPosition, 2, siege_tower_level, tileSize);
//                 towers.push_back(tower);
//                 return;
//             }
//             // Clicked somewhere else to escape the menu.
//             build_buttons = false;
//             return;
//         }
//         {
//             std::cout << "Obstacle clicked" << std::endl;
//             // Do something with the obstacle
//             last_succesful_tile_click = click_coords;
//             build_buttons = true;
//         }
//     }
//     // If build buttons are off and the upgrade button was pressed.
//     // If the build menu is open will result in the menu closing and clicks on the buttons build a tower where previously clicked.
//     if (!build_buttons && upgrade_button)
//     {
//         if (click_coords.x >= 31 && click_coords.x <= 37)
//         {
//             if (clicked_tower_id != -1 && click_coords.y >= 9 && click_coords.y <= 15)
//             {
//                 towers[clicked_tower_id]->upgrade();
//                 std::cout << "upgraded tower of id" << clicked_tower_id << std::endl;
//                 clicked_tower_id = -1;
//                 upgrade_button = false;
//                 return;
//             }
//         }
//     }



// }


















// Takes click_coords in pixels and calculates state of menu and implements button presses.
void TileMap::clickEvents(sf::Vector2i click_coords)
{
    int grid_index_x = click_coords.x/tileSize;
    int grid_index_y = click_coords.y/tileSize;

    std::cout << "grid_index_x = " << grid_index_x << " grid_index_y = " << grid_index_y << std::endl;
    std::cout << "click_coords.x/tileSize = " << click_coords.x/tileSize << " click_coords.y/tileSize = " << click_coords.y/tileSize << std::endl;
    std::cout << "build_buttons = " << build_buttons << " upgrade_button = " << upgrade_button << std::endl;
    

    // checks current state of displayed buttons

    // No menu shown.
    if (!build_buttons && !upgrade_button)
    {
        // If it's a buildable tile, checks if the tile has a tower. If yes, goes to upgrade_button; otherwise, to build_buttons.
        if (grid[grid_index_y][grid_index_x].isBuildable)
        {
            std::cout << "isBuildable clicked." << std::endl;
            
            last_succesful_tile_click = click_coords;   // In pixels. last_succesful_tile_click is an attribute of TileMap.

            // Looks for a tower in the clicked tile.
            sf::Vector2f position_of_possibly_clicked_tower(click_coords.x, click_coords.y); // In pixels.
            // Search the possibly clicked tower's id in towers:
            clicked_tower_id = -1;
            for (size_t i = 0; i < towers.size(); i++)
            {
                // If it's close enough.
                if (std::abs(towers[i]->getPosition().x - position_of_possibly_clicked_tower.x) < 0.5f * tileSize
                &&  std::abs(towers[i]->getPosition().y - position_of_possibly_clicked_tower.y) < 0.5f * tileSize)
                {
                    // A soon as a tower matches the clicked coordinates, TileMap.clicked_tower_id remembers the towers id for the uppgrade button to use.
                    clicked_tower_id = i;
                    upgrade_button = true;
                    return;
                }
            }
            // Clicked an empty buildable tile so it goes to build_buttons.
            build_buttons = true;
            return;
        }
        else
        {
            std::cout << "Empty unbuildable tile clicked." << std::endl;
            // Do nothing with the empty tile
        }
     
    }
    // If build buttons are displayed
    // If the build menu is open will result in the menu closing and clicks on the buttons build a tower where previously clicked.
    // If the build menu is open, clicks outside the build_buttons result in closing the menu and a click on a cartain button results in building a matching tower.
    if (build_buttons && !upgrade_button)
    {
        /*
        button 1:
            y: [,]
            x: [,]
        button 2:
            y: [,]
            x: [,]
        button 3:
            y: [,]
            x: [,]    
        */

        if (click_coords.x >= 31*tileSize && click_coords.x <= 37*tileSize) // click_coord is measured in pixels.
        {
            if (click_coords.y >= build_button_top_anchors[0] && click_coords.y <= build_button_top_anchors[0] + build_button_height)
            {
                std::cout << "Button 1 clicked" << std::endl;
                build_buttons = false;
                // Builds a pierce tower.
                // Gets a Vector2f to the center of the tile indicated by last_succesful_tile_click.
                // last_succesful_tile_click corresponds to a tower click because the current state is build_buttons.
                // Gets the center of the tile by substracting the module 30 of the coordinate from it so that it's placed at the top left corner of the tile (the min values)
                // and adds half a tile to the coordinate. Does this for both coordinates.
                sf::Vector2f startPosition(last_succesful_tile_click.x - last_succesful_tile_click.x%(tileSize) + (tileSize/2),
                last_succesful_tile_click.y - last_succesful_tile_click.y%(tileSize) + (tileSize/2));
                std::shared_ptr<Tower> tower = std::make_shared<Tower>(startPosition, 0, tower_levels[0], tileSize, tower_textures[0]);
                towers.push_back(tower);
                return;

            }
            else if (click_coords.y >= build_button_top_anchors[1] && click_coords.y <= build_button_top_anchors[1] + build_button_height)
            {
                std::cout << "Button 2 clicked" << std::endl;
                build_buttons = false;
                // Builds a magic tower.
                sf::Vector2f startPosition(last_succesful_tile_click.x - last_succesful_tile_click.x%(tileSize) + (tileSize/2),
                last_succesful_tile_click.y - last_succesful_tile_click.y%(tileSize) + (tileSize/2));
                std::shared_ptr<Tower> tower = std::make_shared<Tower>(startPosition, 1, tower_levels[1], tileSize, tower_textures[1]);
                towers.push_back(tower);
                return;

            }
            else if (click_coords.y >= build_button_top_anchors[2] && click_coords.y <= build_button_top_anchors[2] + build_button_height)
            {
                std::cout << "Button 3 clicked" << std::endl;
                build_buttons = false;
                // Builds a siege tower.
                sf::Vector2f startPosition(last_succesful_tile_click.x - last_succesful_tile_click.x%(tileSize) + (tileSize/2),
                last_succesful_tile_click.y - last_succesful_tile_click.y%(tileSize) + (tileSize/2));
                std::shared_ptr<Tower> tower = std::make_shared<Tower>(startPosition, 2, tower_levels[2], tileSize, tower_textures[2]);
                towers.push_back(tower);
                return;

            }
        }
        // Why do this???
        // else // If x coords are right but not y.
        // {
        //     std::cout << "Clicked outside build_buttons" << std::endl;
        //     last_succesful_tile_click = click_coords;
        //     build_buttons = true;
        // }

        // Clicked somewhere else to escape the menu.
        build_buttons = false;
        return;
    }
    // If build buttons are off and the upgrade button was pressed.
    // If the build menu is open will result in the menu closing and clicks on the buttons build a tower where previously clicked.

    if (!build_buttons && upgrade_button)
    {
        if (click_coords.x >= 31*tileSize && click_coords.x <= 37*tileSize)
        {
            // ugrade button is at the same position the as build magic tower button.
            if (click_coords.y >= build_button_top_anchors[1] && click_coords.y <= build_button_top_anchors[1] + build_button_height)
            {
                // Checks current level of the current tower's type to see if further upgrades are possible.
                // If current tower's type allows upgrades.
                int current_tower_type = towers[clicked_tower_id]->tower_type;
                if (tower_levels[current_tower_type] < 2)   // This is 2 because a tower can only be upgraded twice.
                {
                    // Upgrades towers if user clicks within the button and the type isn't fully upgraded yet.
                    for (size_t i = 0; i < towers.size(); i++)
                    {
                        if (towers[i]->tower_type == current_tower_type)
                        {
                            towers[i]->upgrade();
                            std::cout << "upgraded tower of id" << i << std::endl;
                        }
                    }
                    tower_levels[current_tower_type] ++;

                }
                else
                {
                    std::cout << "Couldn't upgrade tower of (id = " << clicked_tower_id << ") and (type = " << towers[clicked_tower_id]->tower_type << ") because is at max level." << std::endl;
                }
                
            }
        }
        // Exits upgrade state no matter the outcome.
        clicked_tower_id = -1;
        upgrade_button = false;
        return;
    }

    std::cout << std::endl;

}



















// Finds the nearest enemy and shoots a bullet( of the given type TO DO!!!)to it.
void TileMap::ShootNearest(sf::Vector2f startPosition, float damage, int tower_type, float fire_range, float projectile_speed, bool power_up_projectile)
{
    // std::cout << "Enters ShootNearest(sf::Vector2f startPosition, float damage, int tower_type)" << std::endl;
    if (enemy_species.size() == 0)
    {
        std::cout <<  "enemy_species.size() == 0. enemy_species uninitialized!!! Exits TileMap::ShootNearest()" << std::endl;
        std::cout <<  "enemy_species.size() == 0. Exits TileMap::ShootNearest()" << std::endl;
        return;
    }
    
    bool one_enemy_found = false;
    for (size_t i = 0; i < enemy_species.size(); i++)
    {
        if (enemy_species[i].size() > 0)
        {
            one_enemy_found = true;
        }
    }
    if (!one_enemy_found)
    {
        // std::cout << "Exit ShootNearest because there are no targets" << std::endl;
        return;
    }
    


    // std::cout << "ShootNearest checkpoint 1" << std::endl;
    // float closest_square_distance_to_enemy = tileSize*tileSize * 30 * 30 * 2; // No enemy will be farther away than this.
    float closest_distance_to_enemy = fire_range; // Takes the square of range of the firing tower. Uses squares to avoid sqrt().

    int closest_distance_enemy_id_i = -1;
    int closest_distance_enemy_id_j = -1;

    // Finds the closest enemy
    for (size_t i = 0; i < enemy_species.size(); i++)
    {
        for (size_t j = 0; j < enemy_species[i].size(); j++)
        {
            float current_distance_to_enemy = std::sqrt(
                (enemy_species[i][j]->getPosition().x - startPosition.x) * (enemy_species[i][j]->getPosition().x - startPosition.x) + 
                (enemy_species[i][j]->getPosition().y - startPosition.y) * (enemy_species[i][j]->getPosition().y - startPosition.y));
                
            if (current_distance_to_enemy < closest_distance_to_enemy)
            {
                closest_distance_to_enemy = current_distance_to_enemy;
                closest_distance_enemy_id_i = i;
                closest_distance_enemy_id_j = j;
            }
        }
    }
    // std::cout << "ShootNearest checkpoint 2. id = (" << closest_distance_enemy_id_i << ", " << closest_distance_enemy_id_j << ")" << std::endl;
    if (closest_distance_enemy_id_i == -1 || closest_distance_enemy_id_j == -1)
    {
        // std::cout << "Exit ShootNearest because no targets are at reach." << std::endl;
        return;
    }
    
    

    std::shared_ptr<Character> target_character = enemy_species[closest_distance_enemy_id_i][closest_distance_enemy_id_j];
    // Projectile projectile(projectile_start_position, target_character, 200.f);



    // std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(startPosition, target_character, 200.f, damage);
    std::shared_ptr<Projectile> projectile;

    // Projectile(sf::Vector2f startPosition, std::shared_ptr<Character> target_character, float speed, float damage);
    switch (tower_type + 3 * power_up_projectile)
    {
    case 0:
        std::cout << "Arrow projectile" << std::endl;
        // Add an Arrow type object that inherits from projectile to the projectiles vector.
        projectile = std::make_shared<Arrow>(startPosition, target_character, projectile_speed, damage);
        break;
    case 1:
        std::cout << "Magic projectile" << std::endl;
        // Add an Arrow type object that inherits from projectile to the projectiles vector.
        projectile = std::make_shared<MagicOrb>(startPosition, target_character, projectile_speed, damage);
        break;
    case 2:
        std::cout << "Siege projectile" << std::endl;
        // Add an Arrow type object that inherits from projectile to the projectiles vector.
        projectile = std::make_shared<ExplosiveShell>(startPosition, target_character, projectile_speed, damage);
        break;
    case 3:
        std::cout << "Poison Arrow projectile" << std::endl;
        // Add an Arrow type object that inherits from projectile to the projectiles vector.
        projectile = std::make_shared<PoisonArrow>(startPosition, target_character, projectile_speed, damage);
        break;        
    case 4:
        std::cout << "Frost Orb projectile" << std::endl;
        // Add an Arrow type object that inherits from projectile to the projectiles vector.
        projectile = std::make_shared<FrostOrb>(startPosition, target_character, projectile_speed, damage);
        break;
    case 5:
        std::cout << "Shock Shell projectile" << std::endl;
        // Add an Arrow type object that inherits from projectile to the projectiles vector.
        projectile = std::make_shared<ShockShell>(startPosition, target_character, projectile_speed, damage);
        break;
        
    default:
        std::cout << "Unknown projectile" << std::endl;
        projectile = std::make_shared<Projectile>(startPosition, target_character, 200.f, damage);
        break;
}


    projectiles.push_back(projectile);
    std::cout << "ShootNearest cp 3" << std::endl;

}

void TileMap::AOEAttack(sf::Vector2f position, bool apply_shock_shell_effect, float damage)
{
    for (size_t i = 0; i < enemy_species.size(); i++)
    {
        for (size_t j = 0; j < enemy_species[i].size(); j++)
        {
            sf::Vector2f enemy_position = enemy_species[i][j]->getPosition();
            float enemy_distance_from_center = std::sqrt((enemy_position.x-position.x)*(enemy_position.x-position.x) + (enemy_position.y-position.y)*(enemy_position.y-position.y));
            if (enemy_distance_from_center < tileSize) // If the enemy is within a 1 tile radius circle 
            {
                enemy_species[i][j]->siege_damage += damage;
                if (apply_shock_shell_effect)
                {
                    enemy_species[i][j]->shock_shell_timer = 60;
                }
                std::cout << "Explosion affected entity (" << i << ", " << j << ")" << std::endl;
            }
            
        }
    }
    
}

void TileMap::AddWave(std::vector<int> new_wave)
{
    std::cout << "Enters AddWave()" << std::endl;
    this->waves_queue.push(new_wave);
    current_round_is_cleared = false;
}

// Marks the end of the wave to genetic_manager and sets current_round_is_cleared = true to tell main to call genWave().
void TileMap::EndWave()
{
    std::cout << "Enteres EndWave" << std::endl;
    genetic_manager.ClearWave();    // Recalculates the best Individuals of each species.
    genetic_manager.ShowBestIndividuals();
    current_round_cleared_enemies = 0;
    current_round_is_cleared = true;
}

void TileMap::SpawnNextEnemy()
{
    // Checks if the round has been completely spawned.
    if (current_round_spawned_enemies >= wave_lenght)
    {
        std::cout << "current_round_spawned_enemies >= wave_lenght is true" << std::endl;
        std::cout << "waves_queue.pop();" << std::endl;
        // Goes to the next set of characters to spawn. 
        waves_queue.pop();
        current_round_spawned_enemies = 0;
    }

    int current_species = waves_queue.front()[current_round_spawned_enemies];

    // Creates and Individual from the current best.
    Individual current_best = genetic_manager.best_individuals[current_species];
    // Randomly mutates stats of the current Individual and increments total_mutations accordingly .
    std::cout << "mutation_chance = " << mutation_chance << std::endl;
    total_mutations += current_best.MutateStats(mutation_chance);
    
    int spawn_location_offset = current_round_spawned_enemies%4;
    auto character = std::make_shared<Character>(sf::Vector2f(15 + 30*spawn_location_offset, 15), current_best, current_species, enemy_textures[current_species]); // Uses an Individual to set the Character's stats.
    // Adds a character to the current species.
    // This moves the spawned characters to the bottom right corner of the map.
    addCharacter(character, current_species);
    std::cout << "Added a character of species " << current_species << std::endl;
    std::cout << "total_mutations = " << total_mutations << std::endl;

    current_round_spawned_enemies ++;
}

bool TileMap::IsCurrentRoundClear()
{
    return current_round_is_cleared;
}

bool TileMap::IsAtLastEnemySpawn()
{
    return current_round_spawned_enemies == wave_lenght;
}


