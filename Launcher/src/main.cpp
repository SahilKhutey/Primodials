// Launcher/src/main.cpp
#include "Shape/Platform/Window.h"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "Starting Shape Launcher...\n";
    Shape::Platform::Window window;
    Shape::Platform::WindowDesc wd;
    wd.title = "Shape Engine Launcher";
    wd.size = {800, 600};
    
    if (!window.create(wd)) {
        std::cerr << "Failed to create launcher window.\n";
        return 1;
    }

    while (true) {
        window.poll_events();
        // UI rendering here
        window.swap_buffers();
    }

    return 0;
}
