#include "Application.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    ShapeEditor::Application app;
    if (!app.Initialize("Primodials Editor", 1280, 720)) {
        return -1;
    }
    
    app.Run();
    return 0;
}
