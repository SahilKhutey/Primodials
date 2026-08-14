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
        // SDL_GetBasePath() always returns a path with a trailing separator.
        // std::filesystem::path::parent_path() on a trailing-slash path only
        // strips the trailing slash on its FIRST call rather than going up a
        // real directory level — so a naive "parent_path().parent_path()"
        // chain intended to go up two directories actually only goes up one.
        // lexically_normal() collapses the trailing slash so parent_path()
        // chains behave the way the three branches below assume.
        std::filesystem::path exe_dir = std::filesystem::path(base_path).lexically_normal();

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
