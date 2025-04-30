#include <SFML/Graphics.hpp>
#include "../include/TileMap.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "A* Pathfinding");
    window.setFramerateLimit(60);

    TileMap map(20, 20, 30); // Grid 20x20, tiles de 30px

    // Configurar obstáculos (ejemplo)
    for (int i = 3; i < 18; ++i) {
        map.setObstacle(i, 10, true);
        map.setObstacle(10, i, true);
    }

    // Calcular camino
    auto path = map.findPath({0, 0}, {19, 19});

    // Bucle principal
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color::White);
        map.draw(window);

        // Dibujar camino (opcional)
        for (const auto& point : path) {
            sf::RectangleShape pathTile(sf::Vector2f(28, 28));
            pathTile.setPosition(point.x * 30 + 1, point.y * 30 + 1);
            pathTile.setFillColor(sf::Color(0, 255, 0, 150)); // Verde semitransparente
            window.draw(pathTile);
        }

        window.display();
    }

    return 0;
}