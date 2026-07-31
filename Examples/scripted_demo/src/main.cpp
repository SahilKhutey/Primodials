#include <iostream>
#include "Shape/Scripting/LuaEngine.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::cout << "Starting Scripted Demo...\n";
    Shape::Scripting::LuaEngine engine;
    engine.initialize();
    engine.execute_string("print('Hello from Lua!')");
    engine.shutdown();
    return 0;
}
