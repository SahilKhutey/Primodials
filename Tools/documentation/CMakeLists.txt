# Tools/documentation/CMakeLists.txt
add_executable(DocGenerator src/main.cpp)
target_link_libraries(DocGenerator PRIVATE ShapeEngine)
if(NOT MSVC)
    target_compile_options(DocGenerator PRIVATE -Wall -Wextra)
endif()
