// tests/final_validation/test_05_cross_platform.cpp
//
// Tests platform-specific aspects and ensures cross-platform compatibility.

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST_CASE("Cross-platform: Build scripts handle all OS", "[validation][xplatform]") {
    if (!fs::exists("build_all.sh")) {
        FAIL("build_all.sh missing");
    }
    
    std::ifstream f("build_all.sh");
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
    
    REQUIRE((content.find("Linux") != std::string::npos || content.find("uname") != std::string::npos));
    REQUIRE((content.find("Windows") != std::string::npos || content.find("windows") != std::string::npos));
    REQUIRE((content.find("Darwin") != std::string::npos || content.find("macos") != std::string::npos));
}

TEST_CASE("Cross-platform: Windows scripts exist", "[validation][xplatform]") {
    REQUIRE(fs::exists("dev/setup_dev.bat"));
    REQUIRE(fs::exists("build_all.bat"));
}

TEST_CASE("Cross-platform: CMake handles all platforms", "[validation][xplatform]") {
    bool hasCMake = fs::exists("CMakeLists.txt") || fs::exists("source/CMakeLists.txt");
    REQUIRE(hasCMake);
    
    std::string path = fs::exists("CMakeLists.txt") ? "CMakeLists.txt" : "source/CMakeLists.txt";
    std::ifstream f(path);
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
    
    REQUIRE(content.find("cmake_minimum_required") != std::string::npos);
    REQUIRE(content.find("project(") != std::string::npos);
    REQUIRE(content.find("SDL3") != std::string::npos);
}
