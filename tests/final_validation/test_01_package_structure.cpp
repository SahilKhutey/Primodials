// tests/final_validation/test_01_package_structure.cpp
//
// Verifies that the package has all required files in correct locations.

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

namespace fs = std::filesystem;

struct ExpectedFile {
    fs::path path;
    size_t minSizeBytes;
    bool required;
    std::string description;
};

TEST_CASE("Package: Root-level files exist", "[validation][structure]") {
    std::vector<ExpectedFile> rootFiles = {
        {fs::path("README.md"), 500, true, "Main package readme"},
        {fs::path("LICENSE"), 500, true, "MIT license"},
        {fs::path("CHANGELOG.md"), 100, true, "Version history"},
        {fs::path("build_all.sh"), 100, true, "Master build script"},
        {fs::path("dev/setup_dev.sh"), 100, true, "Dev setup script"},
        {fs::path("README_DEV.md"), 500, true, "Developer readme"},
    };
    
    for (const auto& file : rootFiles) {
        INFO("Checking: " << file.path.string());
        REQUIRE(fs::exists(file.path));
        if (file.required) {
            auto size = fs::file_size(file.path);
            REQUIRE(size >= file.minSizeBytes);
        }
    }
}

TEST_CASE("Package: Core directory structure", "[validation][structure]") {
    bool hasSource = fs::exists("source/CMakeLists.txt");
    std::string root = hasSource ? "source/" : "";

    REQUIRE((fs::exists("CMakeLists.txt") || fs::exists("source/CMakeLists.txt")));
    REQUIRE((fs::exists("Engine") || fs::exists("source/Engine")));
    REQUIRE((fs::exists("Games") || fs::exists("source/Games")));
    REQUIRE((fs::is_directory(root + "Engine/Core") || fs::is_directory("Engine/Core")));
    REQUIRE((fs::is_directory(root + "Engine/UI") || fs::is_directory("Engine/UI")));
    REQUIRE((fs::is_directory(root + "Engine/Themes") || fs::is_directory("Engine/Themes")));
    REQUIRE((fs::is_directory(root + "Engine/Localization") || fs::is_directory("Engine/Localization")));
    
    // Content
    REQUIRE((fs::is_directory("Content/Themes") || fs::is_directory("source/Content/Themes")));
    REQUIRE((fs::exists("Content/Themes/deep-sea/theme.json") || fs::exists("source/Content/Themes/deep-sea/theme.json")));
    
    // Locales
    REQUIRE((fs::is_directory("Content/Locales") || fs::is_directory("source/Content/Locales")));
    REQUIRE((fs::exists("Content/Locales/localization_keys.json") || fs::exists("Content/Locales/en.json") || fs::exists("source/Content/Locales/en.json")));
}

TEST_CASE("Package: Developer tools", "[validation][structure]") {
    REQUIRE(fs::exists("dev/setup_dev.sh"));
    REQUIRE(fs::exists("dev/setup_dev.bat"));
    
    // Scripts
    REQUIRE(fs::is_directory("dev/scripts"));
    REQUIRE(fs::exists("dev/scripts/install_deps_ubuntu.sh"));
    REQUIRE(fs::exists("dev/scripts/install_deps_macos.sh"));
    
    // Docker
    REQUIRE(fs::exists("dev/docker/Dockerfile.dev"));
    REQUIRE(fs::exists("dev/docker/docker-compose.yml"));
    
    // VS Code
    REQUIRE(fs::is_directory("dev/vscode"));
    REQUIRE(fs::exists("dev/vscode/settings.json"));
    REQUIRE(fs::exists("dev/vscode/tasks.json"));
}

TEST_CASE("Package: Installer sources", "[validation][structure]") {
    REQUIRE(fs::exists("installer/windows/installer.iss"));
    REQUIRE(fs::exists("installer/linux/build_deb.sh"));
    REQUIRE(fs::exists("installer/linux/build_appimage.sh"));
    REQUIRE(fs::exists("installer/macos/build_pkg.sh"));
}

TEST_CASE("Package: README is comprehensive", "[validation][structure]") {
    REQUIRE(fs::exists("README.md"));
    std::ifstream f("README.md");
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
    
    REQUIRE(content.find("Polygonal Primordials") != std::string::npos);
    REQUIRE((content.find("Quick Install") != std::string::npos || 
            content.find("Installation") != std::string::npos));
    REQUIRE(content.find("System Requirements") != std::string::npos);
    REQUIRE(content.find("Build") != std::string::npos);
    REQUIRE(content.find("License") != std::string::npos);
    REQUIRE(content.length() >= 1000);
}
