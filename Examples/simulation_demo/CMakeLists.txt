cmake_minimum_required(VERSION 3.21)

add_executable(simulation_demo
    src/main.cpp
)

target_link_libraries(simulation_demo PRIVATE ShapeEngine SDL3::SDL3)

target_include_directories(simulation_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/Engine
)

if(MSVC)
    target_compile_options(simulation_demo PRIVATE /W3 /permissive- /EHsc)
else()
    target_compile_options(simulation_demo PRIVATE -Wall -Wextra -pedantic)
endif()

# Copy species data to output directory
add_custom_command(TARGET simulation_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data/Species
        $<TARGET_FILE_DIR:simulation_demo>/Data/Species
    COMMENT "Copying species data files"
)
