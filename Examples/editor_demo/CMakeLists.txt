project(shape_demo_editor)
add_executable(shape_demo_editor src/main.cpp)
target_link_libraries(shape_demo_editor PRIVATE 
    ShapeEngine
    ShapeUI
    ShapeEditor
)
