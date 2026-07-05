# Tools/cooker/CMakeLists.txt
add_executable(AssetCooker src/main.cpp)
target_link_libraries(AssetCooker PRIVATE ShapeEngine)
if(NOT MSVC)
    target_compile_options(AssetCooker PRIVATE -Wall -Wextra)
endif()
