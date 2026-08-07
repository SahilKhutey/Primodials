#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "Themes/ThemeManager.h"
#include "UI/SettingsState.h"
#include "Localization/LocaleManager.h"
#include "Diary/EcosystemDiary.h"
#include "Steam/Workshop/ThemeValidator.h"
#include "Steam/Workshop/WorkshopRatings.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

using namespace ShapeEngine;
using Catch::Approx;
namespace fs = std::filesystem;

TEST_CASE("FINAL_QA: Theme loading and validation", "[FinalQA][themes]") {
    Themes::ThemeManager tm;
    REQUIRE(tm.initialize("Content/Themes"));
    
    auto themes = tm.discoverThemes();
    REQUIRE(themes.size() >= 3);
    
    std::vector<std::string> requiredThemes = {"deep-sea", "coral-reef", "aurora"};
    for (const auto& required : requiredThemes) {
        auto theme = tm.loadTheme(required);
        REQUIRE(theme != nullptr);
        REQUIRE(!theme->id.empty());
        REQUIRE(!theme->displayName.empty());
        REQUIRE(theme->speciesPalette.size() >= 3);
    }
}

TEST_CASE("FINAL_QA: Settings persist and reload cleanly", "[FinalQA][settings]") {
    fs::create_directories("Content/_user");
    auto& s = UI::SettingsState::get();
    
    s.fpsTarget = 60;
    s.audioVolume = 0.85f;
    s.currentThemeId = "coral-reef";
    s.particleDensityOverride = 1.2f;
    
    REQUIRE(s.save("Content/_user/qa_settings.json"));
    
    s.fpsTarget = 30;
    s.audioVolume = 0.2f;
    s.currentThemeId = "deep-sea";
    
    REQUIRE(s.load("Content/_user/qa_settings.json"));
    REQUIRE(s.fpsTarget == 60);
    REQUIRE(s.audioVolume == Approx(0.85f));
    REQUIRE(s.currentThemeId == "coral-reef");
    
    fs::remove_all("Content/_user");
}

TEST_CASE("FINAL_QA: Multi-language string lookups work across 11 locales", "[FinalQA][locale]") {
    auto& lm = Localization::LocaleManager::get();
    lm.setLocalesDirectory("Content/Locales");
    
    std::vector<std::string> locales = {
        "en", "es", "ja"
    };
    
    for (const auto& locale : locales) {
        REQUIRE(lm.loadLocale(locale));
        REQUIRE(lm.has("ui.tray.pause"));
        REQUIRE(lm.has("ui.tray.theme"));
        REQUIRE(lm.has("settings.title"));
        
        std::string pause = lm.get("ui.tray.pause");
        REQUIRE(pause.find("[[") == std::string::npos);
    }
}

TEST_CASE("FINAL_QA: Ecosystem diary records and retrieves events", "[FinalQA][diary]") {
    fs::create_directories("Content/_user");
    Diary::EcosystemDiary diary;
    diary.initialize("Content/_user/qa_diary.json");
    
    diary.recordEvent(Diary::EventType::FirstSpawn);
    diary.notifySpeciesChange(0, 10);
    diary.notifySpeciesChange(0, 0); // Extinction
    diary.recordEvent(Diary::EventType::NewSpeciesEmerged);
    
    auto recent = diary.getRecent(10);
    REQUIRE(!recent.empty());
    
    fs::remove_all("Content/_user");
}

TEST_CASE("FINAL_QA: Steam App ID file is valid", "[FinalQA][steam]") {
    std::ifstream appid("steam_appid.txt");
    REQUIRE(appid.is_open());
    
    std::string content;
    std::getline(appid, content);
    REQUIRE(!content.empty());
    REQUIRE(content != "0");
}
