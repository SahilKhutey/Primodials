// tests/final_validation/test_04_build_pipeline.cpp
//
// Tests CMake build configuration and pipeline.

#include <catch2/catch_test_macros.hpp>
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("Build Pipeline: CMakeLists exists", "[validation][build]") {
    REQUIRE((fs::exists("CMakeLists.txt") || fs::exists("source/CMakeLists.txt")));
}

TEST_CASE("Build Pipeline: Build scripts exist", "[validation][build]") {
    REQUIRE((fs::exists("build_all.sh") || fs::exists("build_all.bat")));
    REQUIRE(fs::exists("create_release.sh"));
}
