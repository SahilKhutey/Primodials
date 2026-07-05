# Build ImGui as a static library
add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    backends/imgui_impl_sdl3.cpp
    backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)
target_compile_definitions(imgui PUBLIC
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM=gl3w
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
)
target_link_libraries(imgui PUBLIC SDL3::SDL3-static)
set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)
