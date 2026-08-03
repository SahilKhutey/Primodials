#include <catch2/catch_test_macros.hpp>
#include "Core/Logger.hpp"
#include "Core/Assert.hpp"
#include <filesystem>
#include <fstream>
#include <string>

TEST_CASE("Logger Initialization and Logging File Output", "[Core]") {
    const char* testLogFilename = "test_run.log";
    
    // Clean up old log file if it exists
    if (std::filesystem::exists(testLogFilename)) {
        std::filesystem::remove(testLogFilename);
    }

    Shape::Logger::Initialize(testLogFilename);

    SHAPE_LOG_INFO("Info message: test float {:.2f}", 3.14159f);
    SHAPE_LOG_WARN("Warning message: test int {}", 12345);
    SHAPE_LOG_ERROR("Error message");

    Shape::Logger::Shutdown();

    REQUIRE(std::filesystem::exists(testLogFilename));

    // Verify log file contents
    std::ifstream file(testLogFilename);
    REQUIRE(file.is_open());

    std::string line;
    bool foundInfo = false;
    bool foundWarn = false;
    bool foundError = false;

    while (std::getline(file, line)) {
        if (line.find("[INFO ]") != std::string::npos && line.find("Info message: test float 3.14") != std::string::npos) {
            foundInfo = true;
        }
        if (line.find("[WARN ]") != std::string::npos && line.find("Warning message: test int 12345") != std::string::npos) {
            foundWarn = true;
        }
        if (line.find("[ERROR]") != std::string::npos && line.find("Error message") != std::string::npos) {
            foundError = true;
        }
    }
    file.close();

    REQUIRE(foundInfo);
    REQUIRE(foundWarn);
    REQUIRE(foundError);

    // Clean up
    std::filesystem::remove(testLogFilename);
}
