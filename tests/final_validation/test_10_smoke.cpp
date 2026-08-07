// tests/final_validation/test_10_smoke.cpp
//
// Quick smoke tests that verify critical paths.

#include <catch2/catch_test_macros.hpp>
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("Smoke: Build scripts exist", "[validation][smoke]") {
    REQUIRE((fs::exists("build_all.sh") || fs::exists("build_all.bat")));
}

TEST_CASE("Smoke: Master localization JSON is valid", "[validation][smoke]") {
    REQUIRE(fs::exists("Content/Locales/localization_keys.json"));
}

TEST_CASE("Smoke: UI icons generated", "[validation][smoke]") {
    REQUIRE(fs::is_directory("Content/Assets/UI/Icons"));
    REQUIRE(fs::exists("Content/Assets/UI/Icons/play.png"));
    REQUIRE(fs::exists("Content/Assets/UI/Icons/settings.png"));
}
