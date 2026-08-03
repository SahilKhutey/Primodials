#include <iostream>
#include "Shape/Scripting/LuaEngine.h"

int main(int argc, char* argv[]) {
    std::cout << "Starting Scripted Demo...\n";
    Shape::Scripting::LuaEngine engine;
    engine.initialize();
    engine.execute_string("print('Hello from Lua!')");
    engine.shutdown();
    return 0;
}
