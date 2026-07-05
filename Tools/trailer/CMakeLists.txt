# Tools/trailer/CMakeLists.txt
add_executable(TrailerCamera src/main.cpp)
target_link_libraries(TrailerCamera PRIVATE ShapeEngine)
if(NOT MSVC)
    target_compile_options(TrailerCamera PRIVATE -Wall -Wextra)
endif()
