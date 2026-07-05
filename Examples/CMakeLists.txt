add_executable(BasicWindow
    BasicWindow/Main.cpp
)

target_link_libraries(BasicWindow PRIVATE
    ShapeEngine
)

# Apply static link options to prevent runtime DLL failures
if(NOT MSVC)
    target_link_options(BasicWindow PRIVATE -static)
endif()

add_executable(ScalarExample
    ScalarExample.cpp
)

target_link_libraries(ScalarExample PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(ScalarExample PRIVATE -static)
endif()

add_executable(Vector3Example
    Vector3Example.cpp
)

target_link_libraries(Vector3Example PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(Vector3Example PRIVATE -static)
endif()

add_executable(Matrix2Example
    Matrix2Example.cpp
)

target_link_libraries(Matrix2Example PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(Matrix2Example PRIVATE -static)
endif()

add_executable(QuaternionExample
    QuaternionExample.cpp
)

target_link_libraries(QuaternionExample PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(QuaternionExample PRIVATE -static)
endif()

add_executable(PointExample
    PointExample.cpp
)

target_link_libraries(PointExample PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(PointExample PRIVATE -static)
endif()


add_executable(RenderingExample
    RenderingExample.cpp
)
target_link_libraries(RenderingExample PRIVATE ShapeEngine)
if(NOT MSVC)
    target_link_options(RenderingExample PRIVATE -static)
endif()

add_executable(simulation_demo
    simulation_demo/src/main.cpp
)

target_link_libraries(simulation_demo PRIVATE ShapeEngine SDL3::SDL3)

target_include_directories(simulation_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/Engine
)

if(MSVC)
    target_compile_options(simulation_demo PRIVATE
        /W3 /permissive- /EHsc /fp:precise)
else()
    target_compile_options(simulation_demo PRIVATE -Wall -Wextra -pedantic)
endif()

# Copy species data files to output directory after build
add_custom_command(TARGET simulation_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data/Species
        $<TARGET_FILE_DIR:simulation_demo>/Data/Species
    COMMENT "Copying Species data files"
)

# ─── Phase D: Life Demo ───────────────────────────────────────────────────────

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

add_custom_command(TARGET life_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data/Species
        $<TARGET_FILE_DIR:life_demo>/Data/Species
    COMMENT "Copying Species data files"
)

# ─── Phase E: Demos ───────────────────────────────────────────────────────────
add_subdirectory(editor_demo)
add_subdirectory(replay_demo)
add_subdirectory(civilization_demo)

# ─── Phase F: Demos ───────────────────────────────────────────────────────────
add_subdirectory(vulkan_demo)
add_subdirectory(multiplayer_demo)
add_subdirectory(scripted_demo)
