// Games/PolygonalPrimordials/src/main.cpp
#include "Game.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    PolygonalPrimordials::GameConfig cfg;
    PolygonalPrimordials::Game game(cfg);

    if (!game.initialize()) {
        std::cerr << "Failed to initialize game!\n";
        return 1;
    }

    return game.run();
}
