cmake_minimum_required(VERSION 3.22)
project(ShapeEngine
    VERSION 0.1.0
    DESCRIPTION "Shape Engine — Deterministic Simulation Platform"
    LANGUAGES CXX
)

# Force out-of-source builds
if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Use a build/ directory.")
endif()

# ─── C++ Standard ─────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ─── Build type default ────────────────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        Debug Release RelWithDebInfo MinSizeRel)
endif()

# ─── Options ──────────────────────────────────────────────────────────────────
option(SHAPE_BUILD_TESTS        "Build engine tests"                    ON)
option(SHAPE_BUILD_EXAMPLES     "Build example programs"                ON)
option(SHAPE_BUILD_BENCHMARKS   "Build benchmarks"                      ON)
option(SHAPE_ENABLE_SANITIZER   "Enable AddressSanitizer (Debug only)"  OFF)
option(SHAPE_ENABLE_UBSAN       "Enable UBSanitizer (Debug only)"       OFF)
option(SHAPE_ENABLE_TSAN        "Enable ThreadSanitizer (Debug only)"   OFF)
option(SHAPE_ENABLE_ASSERTS     "Enable runtime asserts"                ON)
option(SHAPE_ENABLE_LOGGING     "Enable logging system"                 ON)

# ─── cmake/ helpers ───────────────────────────────────────────────────────────
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ShapeConfig)
include(CompilerWarnings)
include(Sanitizers)

# ─── Third-party dependencies ─────────────────────────────────────────────────
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

set(SDL_SHARED OFF CACHE BOOL "Build shared SDL library" FORCE)
set(SDL_STATIC ON  CACHE BOOL "Build static SDL library" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS        OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL      OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.8
)
FetchContent_MakeAvailable(SDL3)

# ─── Subdirectories ───────────────────────────────────────────────────────────
add_subdirectory(Engine)

if(SHAPE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(EngineTests)
endif()

if(SHAPE_BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif()

if(SHAPE_BUILD_BENCHMARKS)
    add_subdirectory(Benchmarks)
endif()
