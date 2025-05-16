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



int current_species_to_spawn_offset = 0;
int species_spawn_rates[4] = {15,10,7,7}; // Spawns 15+10+7+7 + 13 enemies per round = 52
int wave_length =             15+10+7+7 + 13;

std::vector<int> genWave()
{
    /* Order of species and spawn ratios:
    0 Orcs, 15
    1 Night elves, 10
    2 Harpies, 7
    3 Mercenaries, 7
    */
    std::vector<int> wave;
    int current_species_to_spawn = (0+current_species_to_spawn_offset)%4;
    for (int i = 0; i < species_spawn_rates[current_species_to_spawn]; ++i)
    {
        int random_25_chance = random_int(0, 3);
        // At a 25% chance inserts a random (not necessarily different) species to the wave.
        if (random_25_chance == 0)
        {
            int num = random_int(0, 3);
            wave.push_back(num);
        }
        else
        {
            wave.push_back(current_species_to_spawn);
        }
    }

    current_species_to_spawn = (1+current_species_to_spawn_offset)%4;
    for (int i = 0; i < species_spawn_rates[current_species_to_spawn]; ++i)
    {
        int random_25_chance = random_int(0, 3);
        // At a 25% chance inserts a random (not necessarily different) species to the wave.
        if (random_25_chance == 0)
        {
            int num = random_int(0, 3);
            wave.push_back(num);
        }
        else
        {
            wave.push_back(current_species_to_spawn);
        }
    }

    current_species_to_spawn = (2+current_species_to_spawn_offset)%4;
    for (int i = 0; i < species_spawn_rates[current_species_to_spawn]; ++i)
    {
        int random_25_chance = random_int(0, 3);
        // At a 25% chance inserts a random (not necessarily different) species to the wave.
        if (random_25_chance == 0)
        {
            int num = random_int(0, 3);
            wave.push_back(num);
        }
        else
        {
            wave.push_back(current_species_to_spawn);
        }
    }

    current_species_to_spawn = (3+current_species_to_spawn_offset)%4;
    for (int i = 0; i < species_spawn_rates[current_species_to_spawn]; ++i)
    {
        int random_25_chance = random_int(0, 3);
        // At a 25% chance inserts a random (not necessarily different) species to the wave.
        if (random_25_chance == 0)
        {
            int num = random_int(0, 3);
            wave.push_back(num);
        }
        else
        {
            wave.push_back(current_species_to_spawn);
        }
    }
    
    // current_species_to_spawn is used differently for the last for. The last 13 enemies of the round are random,
    // but each consecutive one only has a 33% chance of being different than the last one inserted. 
    for (int i = 0; i < 13; ++i)
    {
        int random_33_chance = random_int(0, 2);
        // At a 25% chance inserts a random (not necessarily different) species to the wave.
        if (random_33_chance == 0)
        {
            int num = random_int(0, 3);
            wave.push_back(num);
            current_species_to_spawn = num;
        }
        else
        {
            wave.push_back(current_species_to_spawn);
        }
    }
    
    // Starts next round from a different species.
    current_species_to_spawn_offset += random_int(0, 3);

    return wave;
}



int main() {
    // sf::RenderWindow window(sf::VideoMode(1200, 950), "A* Pathfinding con Personajes");
    sf::RenderWindow window(sf::VideoMode(1200, 950), "Genetic Kingdom");

    std::cout << "Main checkpoint 1" << std::endl;
    int global_speed_multiplier = 1;
    window.setFramerateLimit(60*global_speed_multiplier);
    std::cout << "Main checkpoint 2" << std::endl;
    sf::Clock clock;
    std::cout << "Main checkpoint 3" << std::endl;


    int laberinto[30][30] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}
    };


    TileMap map(30, 30, 30, 52, laberinto);    // 30 is TileSize and also the amount of tiles for each axis. 52 is the wave_length.
    std::cout << "Main checkpoint 4" << std::endl;
    // Configurar obstáculos



    
    // setObstacle() is used only in testing 
    // for (int i = 0; i < 30; ++i) {
    //     // should be 30 but wasn't failing?
    //     for (int j = 0; j < 40; ++j) {
    //         if (laberinto[i][j] == 1) {
    //             map.setObstacle(i, j, true);
    //         }
    //     }
    // }

    // map.setTiles(laberinto);    // Sets corresponding tiles into the map. possible values: {0,1,2}.
    
    // std::cout << "Main checkpoint 5" << std::endl;


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
    // int counterEnemy = 0;
    
    // Adds initial waves to the game. Rounds don't define genetic behaviour of spawned enemies.
    // All genetics are managed by TileMap.genetic_manager of the Genetics class.
    map.AddWave(genWave());
    map.AddWave(genWave());

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        
        // speed multiplying must multiply deltaTime and the max framerate by the same value to work.
        // float global_speed_multiplier = 2.f;
        deltaTime *= global_speed_multiplier;

        bool new_wave_added = false;

        if (map.IsCurrentRoundClear())
        {
            map.AddWave(genWave());
            new_wave_added = true;
            // counter60 = 0;
            global_speed_multiplier ++;
            window.setFramerateLimit(60*global_speed_multiplier);
        }
        

        counter60 ++;
        if (counter60 >= 300)
        {
            // Create a bullet.
            counter60 = 0;
            // std::cout << "deltaTime = " << deltaTime << std::endl;    
            // map.ShootRandomProjectile();
            std::cout << std::endl;
        }
        
        counter600 ++;
        // When a wave is done spawning, it waits until it's clear to call map.SpawnNextEnemy() again.
        if (counter600 >= 30 && (!map.IsAtLastEnemySpawn() || new_wave_added)) 
        {
            // Create a character;
            counter600 = 0;
            // std::cout << "deltaTime = " << deltaTime << std::endl;
            

            // Individual base_orc = Individual(200.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            // auto character = std::make_shared<Character>(sf::Vector2f(15, 15), base_orc, 0); // Uses an Individual to set the Character's stats.
            // // map.addCharacter(character);
            // map.addCharacter(character, 0); // Adds a character to the species of id 0 (orcs).
            
            // Individual base_ne = Individual(200.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            // character = std::make_shared<Character>(sf::Vector2f(45, 15), base_ne, 1); // Uses an Individual to set the Character's stats.
            // // map.addCharacter(character);
            // map.addCharacter(character, 0); // Adds a character to the species of id 0 (orcs).
            
            // Individual base_harpy = Individual(200.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            // character = std::make_shared<Character>(sf::Vector2f(75, 75), base_harpy, 2); // Uses an Individual to set the Character's stats.
            // // map.addCharacter(character);
            // map.addCharacter(character, 0); // Adds a character to the species of id 0 (orcs).
            
            // Individual base_merc = Individual(200.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            // character = std::make_shared<Character>(sf::Vector2f(105, 105), base_merc, 3); // Uses an Individual to set the Character's stats.
            // // map.addCharacter(character);
            // map.addCharacter(character, 0); // Adds a character to the species of id 0 (orcs).

            
            map.SpawnNextEnemy();

            std::cout << std::endl;
            
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
        
        // window.clear(sf::Color::White);
        window.clear(sf::Color(32,32,24));
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





