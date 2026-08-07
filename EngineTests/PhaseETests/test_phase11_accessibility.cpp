// EngineTests/PhaseETests/test_phase11_accessibility.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "UI/Accessibility/KeyboardNavigator.h"
#include "Localization/Localization.h"

TEST_CASE("KeyboardNavigator focus cycling and Tab navigation", "[phase11]") {
    ShapeEngine::UI::KeyboardNavigator nav;
    REQUIRE_FALSE(nav.hasFocus());

    nav.registerElement({1, "Button A", {0, 0, 100, 30}, true, true, nullptr, nullptr});
    nav.registerElement({2, "Button B", {0, 0, 100, 30}, true, true, nullptr, nullptr});
    nav.registerElement({3, "Button C", {0, 0, 100, 30}, true, true, nullptr, nullptr});

    nav.focusElement(1);
    REQUIRE(nav.hasFocus());
    REQUIRE(nav.getFocusedId() == 1);

    nav.focusNext();
    REQUIRE(nav.getFocusedId() == 2);

    nav.focusNext();
    REQUIRE(nav.getFocusedId() == 3);

    nav.focusNext(); // Wrap around to 1
    REQUIRE(nav.getFocusedId() == 1);

    nav.focusPrevious(); // Wrap back to 3
    REQUIRE(nav.getFocusedId() == 3);

    bool activated = false;
    nav.registerElement({4, "Action Button", {0, 0, 100, 30}, true, true, [&]() { activated = true; }, nullptr});
    nav.focusElement(4);
    REQUIRE(nav.handleKey(SDLK_RETURN));
    REQUIRE(activated);

    nav.clearFocus();
    REQUIRE_FALSE(nav.hasFocus());
}

TEST_CASE("Localization key lookup and format string substitution", "[phase11]") {
    auto& loc = ShapeEngine::Localization::Localization::get();
    
    ShapeEngine::Localization::Localization::Config cfg;
    cfg.defaultLocale = "en";
    cfg.localesDirectory = "Content/Locales";
    cfg.autoDetectSystemLocale = false;
    loc.initialize(cfg);

    REQUIRE(loc.getCurrentLocale() == "en");

    loc.setOverride("ui.greeting", "Hello, {}!");
    loc.setOverride("ui.score", "Player {0} achieved {1} points");

    REQUIRE(loc.hasKey("ui.greeting"));
    REQUIRE(loc.get("ui.greeting") == "Hello, {}!");
    REQUIRE(loc.getf("ui.greeting", {std::string("Evolution")}) == "Hello, Evolution!");
    REQUIRE(loc.getf("ui.score", {std::string("Alex"), 9500}) == "Player Alex achieved 9500 points");

    loc.shutdown();
}
