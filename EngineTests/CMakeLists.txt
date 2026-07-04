add_executable(ShapeEngineTests
    Main.cpp
    Core/LoggerTests.cpp
    Core/BootstrapTests.cpp
    Core/JobSystemTests.cpp
    Core/PlatformTests.cpp
    Memory/AllocatorTests.cpp
    Math/MathTests.cpp
    Math/ScalarTests.cpp
    Math/Vector3Tests.cpp
    Math/Matrix2Tests.cpp
    Math/QuaternionTests.cpp
    Math/GeometryTests.cpp
    Math/PointTests.cpp
    ECS/EcsTests.cpp
    Physics/PhysicsTests.cpp
    Biology/BiologyTests.cpp
    AI/CognitiveTests.cpp
    AI/CareerTests.cpp
    Civilization/CivilizationSystemTests.cpp
    Networking/NetworkingTests.cpp
    ContainerTests.cpp
)

target_link_libraries(ShapeEngineTests PRIVATE
    ShapeEngine
    Catch2::Catch2WithMain
)

if(NOT MSVC)
    target_link_options(ShapeEngineTests PRIVATE -static)
endif()

# Target-specific compiler options
if(MSVC)
    target_compile_options(ShapeEngineTests PRIVATE
        /W4          # Warning level 4
        /WX          # Treat warnings as errors
        /permissive- # Standards conformance
        /volatile:iso
        /EHsc
        /fp:precise
    )
else()
    target_compile_options(ShapeEngineTests PRIVATE
        -Wall
        -Wextra
        -Werror
        -pedantic
        -fno-fast-math
        -Wno-c2y-extensions
    )
endif()

# Include the Catch module to discover tests automatically
include(CTest)
enable_testing()

include(Catch)
catch_discover_tests(ShapeEngineTests)
