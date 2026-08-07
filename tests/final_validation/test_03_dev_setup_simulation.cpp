// tests/final_validation/test_03_dev_setup_simulation.cpp
//
// Validates developer environment scripts and configurations.

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST_CASE("Dev Setup: Setup scripts exist", "[validation][dev]") {
    REQUIRE(fs::exists("dev/setup_dev.sh"));
    REQUIRE(fs::exists("dev/setup_dev.bat"));
}

TEST_CASE("Dev Setup: All dependency install scripts exist", "[validation][dev]") {
    REQUIRE(fs::exists("dev/scripts/install_deps_ubuntu.sh"));
    REQUIRE(fs::exists("dev/scripts/install_deps_macos.sh"));
    REQUIRE(fs::exists("dev/scripts/install_deps_windows.ps1"));
}

TEST_CASE("Dev Setup: Python requirements file exists", "[validation][dev]") {
    REQUIRE(fs::exists("dev/requirements.txt"));
    
    std::ifstream f("dev/requirements.txt");
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
    
    REQUIRE((content.find("Pillow") != std::string::npos || 
             content.find("numpy") != std::string::npos));
}

TEST_CASE("Dev Setup: VS Code configuration complete", "[validation][dev]") {
    REQUIRE(fs::is_directory("dev/vscode"));
    
    std::vector<std::string> files = {
        "settings.json", "tasks.json", 
        "extensions.json", "launch.json"
    };
    
    for (const auto& f : files) {
        INFO("Checking: dev/vscode/" + f);
        REQUIRE(fs::exists("dev/vscode/" + f));
    }
}

TEST_CASE("Dev Setup: tasks.json has build task", "[validation][dev]") {
    if (!fs::exists("dev/vscode/tasks.json")) return;
    
    std::ifstream f("dev/vscode/tasks.json");
    std::stringstream ss;
    ss << f.rdbuf();
    std::string content = ss.str();
    
    REQUIRE(content.find("Build") != std::string::npos);
}

TEST_CASE("Dev Setup: Docker setup valid", "[validation][dev]") {
    REQUIRE(fs::exists("dev/docker/Dockerfile.dev"));
    
    std::ifstream f("dev/docker/Dockerfile.dev");
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
    
    REQUIRE(content.find("FROM") != std::string::npos);
    REQUIRE((content.find("apt-get") != std::string::npos || 
             content.find("cmake") != std::string::npos));
    REQUIRE(content.find("WORKDIR") != std::string::npos);
}

TEST_CASE("Dev Setup: docker-compose.yml valid", "[validation][dev]") {
    if (!fs::exists("dev/docker/docker-compose.yml")) return;
    
    std::ifstream f("dev/docker/docker-compose.yml");
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
    
    REQUIRE(content.find("services:") != std::string::npos);
}
