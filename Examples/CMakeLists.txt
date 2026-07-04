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
