#include <catch2/catch_test_macros.hpp>
#include "Steam/Workshop/ThemeValidator.h"
#include "Steam/Workshop/ThemePublisher.h"
#include "Steam/Workshop/WorkshopDiscovery.h"
#include "Steam/Workshop/WorkshopRatings.h"
#include <filesystem>
#include <fstream>

using namespace ShapeEngine;
using namespace ShapeEngine::Steam;
namespace fs = std::filesystem;

TEST_CASE("ThemeValidator catches missing files", "[validator]") {
    fs::create_directories("Content/_test/theme_missing");

    ThemeSubmission sub;
    sub.themeDirectory = "Content/_test/theme_missing";
    sub.title = "Test Theme";
    sub.description = "A test theme description that is long enough";
    sub.tags = { "test" };

    ThemeValidator v;
    auto issues = v.validate(sub);

    REQUIRE_FALSE(v.canPublish(issues));

    fs::remove_all("Content/_test/theme_missing");
}

TEST_CASE("ThemeValidator passes valid theme directory", "[validator]") {
    fs::create_directories("Content/_test/theme_valid");

    {
        std::ofstream f("Content/_test/theme_valid/theme.json");
        f << "{\"id\": \"test-valid\", \"displayName\": \"Valid Test\"}";
    }
    {
        std::ofstream f("Content/_test/theme_valid/preview.jpg");
        f << "dummy_preview";
    }
    {
        std::ofstream f("Content/_test/theme_valid/title.png");
        f << "dummy_title";
    }

    ThemeSubmission sub;
    sub.themeDirectory = "Content/_test/theme_valid";
    sub.title = "Valid Test Theme";
    sub.description = "This is a valid test theme with all required metadata filled in.";
    sub.tags = { "test", "ambient" };

    ThemeValidator v;
    auto issues = v.validate(sub);
    REQUIRE(v.canPublish(issues));

    fs::remove_all("Content/_test/theme_valid");
}

TEST_CASE("ThemePublisher reports progress and completes", "[publisher]") {
    fs::create_directories("Content/_test/theme_pub");
    {
        std::ofstream f("Content/_test/theme_pub/theme.json");
        f << "{\"id\": \"test-pub\", \"displayName\": \"Pub Test\"}";
    }
    {
        std::ofstream f("Content/_test/theme_pub/preview.jpg");
        f << "preview";
    }
    {
        std::ofstream f("Content/_test/theme_pub/title.png");
        f << "title";
    }

    ThemeSubmission sub;
    sub.themeDirectory = "Content/_test/theme_pub";
    sub.title = "Valid Publish Theme";
    sub.description = "Full description for publishing.";
    sub.tags = { "publish" };

    ThemePublisher publisher;
    bool completed = false;
    bool success = false;

    publisher.setOnCompleted([&](bool s, uint64_t id, const std::string& err) {
        (void)id;
        (void)err;
        completed = true;
        success = s;
    });

    REQUIRE(publisher.startPublish(sub));
    REQUIRE(completed);
    REQUIRE(success);

    fs::remove_all("Content/_test/theme_pub");
}

TEST_CASE("WorkshopRatings records ratings", "[ratings]") {
    WorkshopRatings ratings;
    REQUIRE(ratings.rate(1001, 5));
    REQUIRE(ratings.getAverageRating(1001) == 5.0f);
    REQUIRE(ratings.getRatingCount(1001) == 1);
}
