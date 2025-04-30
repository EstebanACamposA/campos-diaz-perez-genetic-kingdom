cmake_minimum_required(VERSION 3.15)
project(AStarPathfinder LANGUAGES CXX)

# Configuración básica
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar SFML (requiere Graphics module)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/TileMap.cpp
)

# Archivos de cabecera
include_directories(include)

# Ejecutable principal
add_executable(AStar ${SOURCES})

# Vincular SFML
target_link_libraries(AStar
    sfml-graphics
    sfml-window
    sfml-system
)

# Configuración para instalación (opcional)
install(TARGETS AStar DESTINATION bin)