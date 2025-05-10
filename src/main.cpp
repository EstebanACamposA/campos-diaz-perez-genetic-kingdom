#include <SFML/Graphics.hpp>
#include "TileMap.hpp"
#include "Character.hpp"
#include <vector>
#include <iostream>
#include <random> // Para std::random_device, std::mt19937 y std::uniform_int_distribution
// SFML logic for starting window and keeping time is here. Tile size defined here. VERIFY THIS!!!
// Función que genera un número aleatorio entre min y max (ambos inclusive)
int random_int(int min, int max) {
    // Generador de números aleatorios (semilla aleatoria basada en hardware)
    static std::random_device rd;
    static std::mt19937 gen(rd()); // Motor de generación Mersenne Twister

    // Distribución uniforme en el rango [min, max]
    std::uniform_int_distribution<int> distrib(min, max);

    return distrib(gen);
}
/*
float random_int() {
    // Generador de números aleatorios (semilla aleatoria basada en hardware)
    static std::random_device rd;
    static std::mt19937 gen(rd()); // Motor de generación Mersenne Twister

    // Distribución uniforme en el rango [min, max]
    std::uniform_int_distribution<float> distrib(0, 1);

    return distrib(gen);
}*/

std::vector<int> genWave(){
    std::vector<int> wave;
    
    for (int i = 0; i < 41; ++i) {
        int num = random_int(0, 3);
        wave.push_back(num);
    
    }
    return wave;
}
int main() {
    std::cout << "Enters main??????" << std::endl;
    sf::RenderWindow window(sf::VideoMode(1200, 950), "A* Pathfinding con Personajes");
    std::cout << "Main checkpoint 1" << std::endl;
    window.setFramerateLimit(60);
    std::cout << "Main checkpoint 2" << std::endl;
    sf::Clock clock;
    std::cout << "Main checkpoint 3" << std::endl;
    TileMap map(30, 30, 30);    // 30 doesn't divde both 800 and 600.
    std::cout << "Main checkpoint 4" << std::endl;
    // Configurar obstáculos

    int laberinto[30][30] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}
};

    

    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 40; ++j) {
            if (laberinto[i][j] == 1) {
                map.setObstacle(i, j, true);
            }
        }
        
    }
    
    std::cout << "Main checkpoint 5" << std::endl;
    // // Characters are now stored into one of the species.
    // // Crear personaje
    // Individual base_orc = Individual(200.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    // auto character = std::make_shared<Character>(sf::Vector2f(15, 15), base_orc, 0); // Uses an Individual to set the Character's stats.
    // // map.addCharacter(character);
    // map.addCharacter(character, 0); // Adds a character to the species of id 0 (orcs).
    
    // Mover al hacer clic
    sf::Vector2i targetPos;
    bool hasTarget = false;

    int counter60 = 99999;
    int counter600 = 99999;
    int counterWaves = 0;
    int counterEnemy = 0;
    
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        counter60 ++;
        if (counter60 >= 300)
        {
            // Create a bullet.
            counter60 = 0;
            // std::cout << "deltaTime = " << deltaTime << std::endl;    
            // map.ShootRandomProjectile();
        }
        
        counter600 ++;
        if (counter600 >= 300)
        {
            // Create a character;
            counter600 = 0;
            // std::cout << "deltaTime = " << deltaTime << std::endl;    
            
            Individual base_orc = Individual(200.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            auto character = std::make_shared<Character>(sf::Vector2f(15, 15), base_orc, 0); // Uses an Individual to set the Character's stats.
            // map.addCharacter(character);
            map.addCharacter(character, 0); // Adds a character to the species of id 0 (orcs).
            
            Individual base_ne = Individual(200.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            character = std::make_shared<Character>(sf::Vector2f(45, 15), base_ne, 1); // Uses an Individual to set the Character's stats.
            // map.addCharacter(character);
            map.addCharacter(character, 0); // Adds a character to the species of id 0 (orcs).
            
            Individual base_harpy = Individual(200.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            character = std::make_shared<Character>(sf::Vector2f(75, 75), base_harpy, 2); // Uses an Individual to set the Character's stats.
            // map.addCharacter(character);
            map.addCharacter(character, 0); // Adds a character to the species of id 0 (orcs).
            
            Individual base_merc = Individual(200.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            character = std::make_shared<Character>(sf::Vector2f(105, 105), base_merc, 3); // Uses an Individual to set the Character's stats.
            // map.addCharacter(character);
            map.addCharacter(character, 0); // Adds a character to the species of id 0 (orcs).
            
        }


        // Wave random int generation.
        // std::vector<int> wave = genWave();
        // printf("wave = ");
        // for (int i = 0; i<41; ++i){
        //     std::cout<<wave[i]<<",";
        // }
        // std::cout<<std::endl;



        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // targetPos = {
                    //     event.mouseButton.x / 30,   // event.mouseButton/tilesize
                    //     event.mouseButton.y / 30
                    // };
                    // // map.moveCharacterTo(character, targetPos); // Testing function.
                    // map.clickEvents(targetPos);
                    sf::Vector2i target_pos_pixels(event.mouseButton.x, event.mouseButton.y);
                    map.clickEvents(target_pos_pixels);
                    
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





