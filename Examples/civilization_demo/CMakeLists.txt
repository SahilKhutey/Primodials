project(shape_demo_civilization)
add_executable(shape_demo_civilization src/main.cpp)
target_link_libraries(shape_demo_civilization PRIVATE 
    ShapeEngine
    ShapeUI
    ShapeProcedural
)
