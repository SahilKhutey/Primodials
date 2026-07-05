# Launcher/CMakeLists.txt
add_executable(ShapeLauncher src/main.cpp)

target_link_libraries(ShapeLauncher PRIVATE ShapeEngine)

if(NOT MSVC)
    target_compile_options(ShapeLauncher PRIVATE -Wall -Wextra)
else()
    target_compile_options(ShapeLauncher PRIVATE /W3 /EHsc)
endif()
