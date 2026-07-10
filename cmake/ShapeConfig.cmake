# cmake/ShapeConfig.cmake
# Project-wide configuration: output directories, compile definitions,
# and helper functions for defining Shape Engine modules.

include_guard(GLOBAL)

# ─── Output directories ───────────────────────────────────────────────────────
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ─── Compile definitions from options ─────────────────────────────────────────
if(SHAPE_ENABLE_ASSERTS)
    add_compile_definitions(SHAPE_ASSERTS_ENABLED=1)
else()
    add_compile_definitions(SHAPE_ASSERTS_ENABLED=0)
endif()

if(SHAPE_ENABLE_LOGGING)
    add_compile_definitions(SHAPE_LOGGING_ENABLED=1)
else()
    add_compile_definitions(SHAPE_LOGGING_ENABLED=0)
endif()

# ─── Helper: add a Shape module static library ────────────────────────────────
# Usage:
#   shape_add_module(MyModule
#       src/MyFile.cpp
#       src/OtherFile.cpp
#   )
function(shape_add_module name)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    if(ARG_UNPARSED_ARGUMENTS)
        add_library(${name} STATIC ${ARG_UNPARSED_ARGUMENTS})
    else()
        add_library(${name} INTERFACE)
    endif()
    target_include_directories(${name} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine>
    )
    if(NOT ARG_UNPARSED_ARGUMENTS)
        return()
    endif()
    target_link_libraries(${name} PUBLIC ShapeEngine)
    shape_apply_warnings(${name})
    shape_apply_sanitizers(${name})
endfunction()
