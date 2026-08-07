# tests/final_validation/CMakeLists.txt

target_sources(ShapeEngineTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/test_01_package_structure.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_02_installer_simulation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_03_dev_setup_simulation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_04_build_pipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_05_cross_platform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_06_user_experience.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_08_performance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_09_security.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_10_smoke.cpp
)
