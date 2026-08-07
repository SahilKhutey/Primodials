# Engine/Scripting/CMakeLists.txt
add_library(ShapeScripting STATIC
    "include/Shape/Scripting/IScriptEngine.h"
    "include/Shape/Scripting/LuaEngine.h"
    "include/Shape/Scripting/ScriptBindings.h"
    "include/Shape/Scripting/ScriptEntity.h"
    "include/Shape/Scripting/ScriptWorld.h"
    "src/LuaEngine.cpp"
    "src/ScriptBindings.cpp"
)

target_include_directories(ShapeScripting PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine>
)

target_link_libraries(ShapeScripting PUBLIC ShapeEngine)
