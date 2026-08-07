// tests/final_validation/test_06_user_experience.cpp
//
// Tests that user-facing documentation and controls are properly set up.

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

TEST_CASE("UX: README has clear quick start", "[validation][ux]") {
    std::ifstream f("README.md");
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
    
    size_t quickStartPos = content.find("Quick Install");
    size_t devPos = content.find("For Developers");
    
    if (quickStartPos != std::string::npos && devPos != std::string::npos) {
        REQUIRE(quickStartPos < devPos);
    }
    REQUIRE(content.find("System Requirements") != std::string::npos);
}

TEST_CASE("UX: System requirements are clear", "[validation][ux]") {
    std::ifstream f("README.md");
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
    
    REQUIRE((content.find("System Requirements") != std::string::npos || content.find("Windows") != std::string::npos || content.find("CPU") != std::string::npos));
}

TEST_CASE("UX: Accessibility checklist exists", "[validation][ux]") {
    REQUIRE(fs::exists("accessibility_checklist.md"));
}

TEST_CASE("UX: Ship readiness report exists", "[validation][ux]") {
    REQUIRE(fs::exists("ship_readiness_report.md"));
}
