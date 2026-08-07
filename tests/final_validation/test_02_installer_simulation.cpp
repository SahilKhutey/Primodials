// tests/final_validation/test_02_installer_simulation.cpp
//
// Tests that installer scripts exist and are properly structured.

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

TEST_CASE("Installer: Build script exists", "[validation][installer]") {
    REQUIRE((fs::exists("build_all.sh") || fs::exists("build_all.bat")));
}

TEST_CASE("Installer: Linux .deb script exists", "[validation][installer]") {
    REQUIRE(fs::exists("installer/linux/build_deb.sh"));
}

TEST_CASE("Installer: Inno Setup script is valid", "[validation][installer]") {
    REQUIRE(fs::exists("installer/windows/installer.iss"));
    
    std::ifstream f("installer/windows/installer.iss");
    std::stringstream ss;
    ss << f.rdbuf();
    std::string content = ss.str();
    
    REQUIRE(content.find("[Setup]") != std::string::npos);
    REQUIRE(content.find("AppName=") != std::string::npos);
    REQUIRE(content.find("AppVersion=") != std::string::npos);
    REQUIRE(content.find("DefaultDirName=") != std::string::npos);
}

TEST_CASE("Installer: macOS pkgbuild script exists", "[validation][installer]") {
    REQUIRE(fs::exists("installer/macos/build_pkg.sh"));
}

TEST_CASE("Installer: All platform installer scripts present", "[validation][installer]") {
    std::vector<std::string> scripts = {
        "build_all.sh",
        "create_release.sh",
        "dev/setup_dev.sh",
        "installer/linux/build_deb.sh",
        "installer/linux/build_appimage.sh",
        "installer/macos/build_pkg.sh",
        "dev/docker/build_in_docker.sh"
    };
    
    for (const auto& script : scripts) {
        INFO("Checking: " << script);
        REQUIRE(fs::exists(script));
    }
}
