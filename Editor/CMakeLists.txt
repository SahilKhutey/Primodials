project(ShapeEditor)

# Fetch ImGui (Docking Branch)
include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

# Compile ImGui as a static library
add_library(ImGui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

# ImGui needs to know about SDL3 headers
target_include_directories(ImGui PUBLIC 
    ${imgui_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(ImGui PUBLIC SDL3::SDL3)

# Editor Executable
add_executable(ShapeEditor
    Main.cpp
    Application.cpp
    EntityInspector.cpp
    SystemProfiler.cpp
    Renderer2D.cpp
)

target_include_directories(ShapeEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ShapeEditor PRIVATE
    ShapeEngine
    ImGui
    SDL3::SDL3
)
