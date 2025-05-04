#include <SFML/Graphics.hpp>
#include "TileMap.hpp"
#include "Character.hpp"

// SFML logic for starting window and keeping time is here. Tile size defined here. VERIFY THIS!!!
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "A* Pathfinding con Personajes");
    sf::Clock clock;
    
    TileMap map(20, 20, 30);    // 30 doesn't divde both 800 and 600.
    
    // Configurar obstáculos
    for (int i = 5; i < 15; ++i) {
        map.setObstacle(i, 10, true);
    }
    
    // Crear personaje
    auto character = std::make_shared<Character>(sf::Vector2f(15, 15));
    map.addCharacter(character);
    
    // Mover al hacer clic
    sf::Vector2i targetPos;
    bool hasTarget = false;

    int counter60 = 0;

    
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        counter60 ++;
        if (counter60 >= 60)
        {
            std::cout << "deltaTime = " << deltaTime << std::endl;    
            map.ShootRandomProjectile();
        }
        
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    targetPos = {
                        event.mouseButton.x / 30,   // event.mouseButton/tilesize
                        event.mouseButton.y / 30
                    };
                    map.moveCharacterTo(character, targetPos);
                }
            }
        }
        
        map.update(deltaTime);
        
        window.clear(sf::Color::White);
        map.draw(window);
        
        // Dibujar objetivo
        if (hasTarget) {
            sf::CircleShape target(10.f);
            target.setPosition(targetPos.x * 30 + 5, targetPos.y * 30 + 5);
            target.setFillColor(sf::Color::Red);
            window.draw(target);
        }
        
        // character->draw(window); // Moved to TileMap.draw
        window.display();
    }
    
    return 0;
}