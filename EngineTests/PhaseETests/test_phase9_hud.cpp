// EngineTests/PhaseETests/test_phase9_hud.cpp
#include <catch2/catch_test_macros.hpp>
#include "UI/HUD/EntityInspector.h"
#include "UI/Widgets/NotificationToast.h"
#include "UI/Profiles/ProfileManager.h"
#include "GameView/GameViewWindow.h"
#include "GameView/SharedStateSync.h"
#include "UI/Widgets/ColorPicker.h"

TEST_CASE("EntityInspector selection and visibility", "[phase9]") {
    ShapeEngine::UI::EntityInspector::Config cfg;
    ShapeEngine::UI::EntityInspector inspector(cfg);

    REQUIRE_FALSE(inspector.isVisible());
    inspector.show();
    REQUIRE(inspector.isVisible());

    inspector.setSelectedEntity(101);
    REQUIRE(inspector.getSelectedEntity() == 101);
    
    inspector.clearSelection();
    REQUIRE(inspector.getSelectedEntity() == ShapeEngine::UI::INVALID_ENTITY);
}

TEST_CASE("NotificationToast message queuing and clear", "[phase9]") {
    ShapeEngine::UI::NotificationToast::Config cfg;
    ShapeEngine::UI::NotificationToast toasts(cfg);

    REQUIRE(toasts.getActiveCount() == 0);
    toasts.info("Title 1", "Message 1");
    toasts.success("Title 2", "Message 2");
    REQUIRE(toasts.getActiveCount() == 2);

    toasts.clear();
    REQUIRE(toasts.getActiveCount() == 0);
}

TEST_CASE("ProfileManager profile save, load, and delete", "[phase9]") {
    ShapeEngine::UI::ProfileManager::Config cfg;
    ShapeEngine::UI::ProfileManager manager(cfg);

    REQUIRE_FALSE(manager.getProfiles().empty());
    REQUIRE(manager.saveCurrentAsProfile("TestProfileUnit"));
    
    REQUIRE(manager.loadProfile("TestProfileUnit"));
    REQUIRE(manager.getActiveProfile() == "TestProfileUnit");

    REQUIRE(manager.deleteProfile("TestProfileUnit"));
}

TEST_CASE("GameViewWindow control state and action callback", "[phase9]") {
    ShapeEngine::GameView::GameViewWindow::Config cfg;
    ShapeEngine::GameView::GameViewWindow gv(cfg);

    REQUIRE_FALSE(gv.isVisible());
    gv.show();
    REQUIRE(gv.isVisible());

    bool actionReceived = false;
    ShapeEngine::GameView::GameViewAction receivedAction = ShapeEngine::GameView::GameViewAction::None;
    gv.setOnAction([&](ShapeEngine::GameView::GameViewAction act) {
        actionReceived = true;
        receivedAction = act;
    });

    gv.setSimulationSpeed(2.0f);
    gv.setEntityCount(250000);
    gv.hide();
    REQUIRE_FALSE(gv.isVisible());
}

TEST_CASE("SharedStateSync light snapshot and command queue", "[phase9]") {
    ShapeEngine::GameView::SharedStateSync sync;
    sync.initialize(nullptr, ShapeEngine::GameView::SharedStateSync::SyncMode::SingleProcess);

    auto snapshot = sync.takeLightSnapshot();
    REQUIRE(snapshot.entityCount == 0);

    ShapeEngine::GameView::SharedStateSync::Command cmd;
    cmd.type = ShapeEngine::GameView::SharedStateSync::Command::Type::PauseSimulation;
    sync.queueCommand(cmd);
    auto cmds = sync.getPendingCommands();
    REQUIRE(cmds.size() == 1);
    REQUIRE(cmds[0].type == ShapeEngine::GameView::SharedStateSync::Command::Type::PauseSimulation);

    sync.clearCommands();
    REQUIRE(sync.getPendingCommands().empty());
}

TEST_CASE("ColorPicker HSV/RGB conversion", "[phase9]") {
    ShapeEngine::UI::ColorPicker::Config cfg;
    ShapeEngine::UI::ColorPicker picker(cfg);

    ShapeEngine::UI::ColorPickerRGBA col(1.0f, 0.0f, 0.0f, 1.0f);
    picker.renderSwatch(10, 10, 20, col);
    REQUIRE(col.r == 1.0f);
}
