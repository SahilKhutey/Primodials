# Tools/localization/CMakeLists.txt
add_executable(LocTool src/main.cpp)
target_link_libraries(LocTool PRIVATE ShapeEngine)
if(NOT MSVC)
    target_compile_options(LocTool PRIVATE -Wall -Wextra)
endif()
