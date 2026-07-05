project(shape_demo_replay)
add_executable(shape_demo_replay src/main.cpp)
target_link_libraries(shape_demo_replay PRIVATE 
    ShapeEngine
    ShapeUI
    ShapeReplay
)
