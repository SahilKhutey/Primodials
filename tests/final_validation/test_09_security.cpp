// tests/final_validation/test_09_security.cpp
//
// Basic security validation for the package.

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

namespace fs = std::filesystem;

TEST_CASE("Security: No hardcoded credentials in source", "[validation][security]") {
    std::vector<std::string> suspiciousPatterns = {
        "AWS_ACCESS_KEY_ID", "PRIVATE_KEY_BEGIN", "-----BEGIN RSA PRIVATE KEY-----"
    };
    
    int found = 0;
    std::string root = fs::exists("Engine") ? "Engine" : "source/Engine";
    
    if (fs::exists(root)) {
        for (auto& entry : fs::recursive_directory_iterator(root)) {
            if (!entry.is_regular_file()) continue;
            if (entry.path().extension() != ".cpp" && 
                entry.path().extension() != ".h") continue;
            
            std::ifstream f(entry.path());
            std::string content((std::istreambuf_iterator<char>(f)),
                                 std::istreambuf_iterator<char>());
            
            for (const auto& pattern : suspiciousPatterns) {
                if (content.find(pattern) != std::string::npos) {
                    found++;
                }
            }
        }
    }
    
    REQUIRE(found == 0);
}
