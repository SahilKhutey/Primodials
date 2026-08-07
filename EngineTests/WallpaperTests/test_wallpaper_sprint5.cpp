#include <catch2/catch_test_macros.hpp>
#include "Localization/LocaleManager.h"
#include <filesystem>
#include <fstream>

using namespace ShapeEngine::Localization;
namespace fs = std::filesystem;

TEST_CASE("LocaleManager loads English dictionary", "[locale]") {
    fs::create_directories("Content/_test_locales");
    {
        std::ofstream f("Content/_test_locales/en.json");
        f << "{\"ui.welcome\": \"Welcome\", \"ui.quit\": \"Quit\"}";
    }

    auto& lm = LocaleManager::get();
    lm.setLocalesDirectory("Content/_test_locales");
    REQUIRE(lm.loadLocale("en"));
    REQUIRE(lm.get("ui.welcome") == "Welcome");
    REQUIRE(lm.get("ui.quit") == "Quit");

    fs::remove_all("Content/_test_locales");
}

TEST_CASE("LocaleManager falls back to English when key is missing", "[locale]") {
    fs::create_directories("Content/_test_locales");
    {
        std::ofstream f("Content/_test_locales/en.json");
        f << "{\"ui.welcome\": \"Welcome\", \"ui.quit\": \"Quit\"}";
    }
    {
        std::ofstream f("Content/_test_locales/es.json");
        f << "{\"ui.welcome\": \"Bienvenido\"}";
    }

    auto& lm = LocaleManager::get();
    lm.setLocalesDirectory("Content/_test_locales");
    REQUIRE(lm.loadLocale("en"));
    REQUIRE(lm.loadLocale("es"));
    REQUIRE(lm.get("ui.welcome") == "Bienvenido");
    REQUIRE(lm.get("ui.quit") == "Quit");

    fs::remove_all("Content/_test_locales");
}

TEST_CASE("Missing translation key returns formatted placeholder", "[locale]") {
    fs::create_directories("Content/_test_locales");
    {
        std::ofstream f("Content/_test_locales/en.json");
        f << "{}";
    }

    auto& lm = LocaleManager::get();
    lm.setLocalesDirectory("Content/_test_locales");
    lm.loadLocale("en");
    REQUIRE(lm.get("missing.key") == "[[missing.key]]");

    fs::remove_all("Content/_test_locales");
}
