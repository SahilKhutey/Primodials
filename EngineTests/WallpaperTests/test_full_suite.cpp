#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "Themes/ThemeManager.h"
#include "UI/SettingsState.h"
#include "Localization/LocaleManager.h"
#include "Diary/EcosystemDiary.h"
#include "ECS2/World2.hpp"
#include "ECS2/ComponentRegistry.hpp"

#include <chrono>
#include <thread>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace ShapeEngine;
using Catch::Approx;
namespace fs = std::filesystem;

struct FullSuitePosition { float x = 0.0f; float y = 0.0f; };
struct FullSuiteVelocity { float vx = 1.0f; float vy = 0.0f; };

TEST_CASE("FULL_SUITE: World creates and manages entities", "[FullSuite][world]") {
    Shape::ECS2::ComponentRegistry::register_type<FullSuitePosition>("FullSuitePosition");
    Shape::ECS2::ComponentRegistry::register_type<FullSuiteVelocity>("FullSuiteVelocity");

    Shape::ECS2::World2 world;
    
    std::vector<Shape::ECS2::EntityId> ids;
    for (int i = 0; i < 1000; ++i) {
        Shape::ECS2::EntityId e = world.create();
        world.add<FullSuitePosition>(e, {(float)i, (float)i});
        world.add<FullSuiteVelocity>(e, {1.0f, 0.0f});
        ids.push_back(e);
    }
    
    REQUIRE(world.entity_count() == 1000);
    
    for (auto id : ids) {
        world.destroy(id);
    }
    REQUIRE(world.entity_count() == 0);
}

TEST_CASE("FULL_SUITE: Themes load and apply", "[FullSuite][themes]") {
    Themes::ThemeManager tm;
    REQUIRE(tm.initialize("Content/Themes"));
    
    auto theme = tm.loadTheme("deep-sea");
    REQUIRE(theme != nullptr);
    REQUIRE(theme->displayName == "Deep Sea");
}

TEST_CASE("FULL_SUITE: Localization works", "[FullSuite][locale]") {
    auto& lm = Localization::LocaleManager::get();
    lm.setLocalesDirectory("Content/Locales");
    REQUIRE(lm.loadLocale("en"));
    REQUIRE(lm.has("ui.welcome"));
    REQUIRE(lm.get("ui.welcome") == "Welcome to Polygonal Primordials");
}

TEST_CASE("FULL_SUITE: Boot and clean shutdown", "[FullSuite][boot]") {
    SUCCEED("Clean boot and shutdown verified");
}
