// Games/PolygonalPrimordials/src/main.cpp
#include "Game.hpp"
#include <SDL3/SDL.h>
#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Automatic asset path resolution
    const char* base_path = SDL_GetBasePath();
    if (base_path) {
        std::filesystem::path exe_dir(base_path);

        if (std::filesystem::exists(exe_dir / "Content") || std::filesystem::exists(exe_dir / "data")) {
            std::filesystem::current_path(exe_dir);
        } else if (std::filesystem::exists(exe_dir.parent_path() / "Content")) {
            std::filesystem::current_path(exe_dir.parent_path());
        } else if (std::filesystem::exists(exe_dir.parent_path().parent_path() / "Content")) {
            std::filesystem::current_path(exe_dir.parent_path().parent_path());
        }
    }

    PolygonalPrimordials::GameConfig cfg;
    PolygonalPrimordials::Game game(cfg);

    if (!game.initialize()) {
        std::cerr << "Failed to initialize game!\n";
        return 1;
    }

    return game.run();
}
