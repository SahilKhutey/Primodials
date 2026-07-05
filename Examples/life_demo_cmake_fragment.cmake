add_executable(life_demo
    life_demo/src/main.cpp
)

target_link_libraries(life_demo PRIVATE ShapeEngine SDL3::SDL3)

target_include_directories(life_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/Engine
)

if(MSVC)
    target_compile_options(life_demo PRIVATE
        /W3 /permissive- /EHsc /fp:precise)
else()
    target_compile_options(life_demo PRIVATE -Wall -Wextra -pedantic)
endif()

# Copy species data files to output directory after build
add_custom_command(TARGET life_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data/Species
        $<TARGET_FILE_DIR:life_demo>/Data/Species
    COMMENT "Copying Species data files"
)
