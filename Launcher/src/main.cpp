// Launcher/src/main.cpp
#include "Platform/Window.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "Starting Shape Launcher...\n";
    Shape::Window window("Shape Engine Launcher", 800, 600);

    while (window.ProcessEvents()) {
        // UI rendering here
        window.SwapBuffers();
    }

    return 0;
}
