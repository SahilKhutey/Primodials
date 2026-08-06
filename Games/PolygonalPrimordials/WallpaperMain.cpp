#include <SDL3/SDL.h>

#include "Wallpaper/SystemTray.h"
#include "Wallpaper/WallpaperWindow.h"
#include "Wallpaper/PowerManager.h"
#include "Themes/ThemeManager.h"
#include "Themes/ThemeApplier.h"
#include "UI/SettingsState.h"
#include "UI/SettingsWindow.h"
#include "Renderer/BackgroundRenderer.h"
#include "Renderer/Renderers/ParticleRenderer.h"
#include "Particles/AmbientParticleSystem.hpp"
#include "Audio/AmbientPlayer.hpp"
#include "GameView/SharedSimulationState.h"
#include "GameView/GameViewWindow.h"
#include "Diary/EcosystemDiary.h"
#include "Steam/Workshop/WorkshopThemeManager.h"
#include "AutoStart/AutoStartManager.h"
#include "Core/Logger.hpp"
#include "Math/Vec4.hpp"

#include <memory>
#include <thread>
#include <chrono>

using namespace ShapeEngine;
using namespace ShapeEngine::Wallpaper;

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        SHAPE_LOG_ERROR("SDL_Init failed: {}", SDL_GetError());
        return 1;
    }

    int screenW = 1920;
    int screenH = 1080;
    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
    if (mode) {
        screenW = mode->w;
        screenH = mode->h;
    }

    auto tray = std::make_unique<SystemTray>();
    auto wallpaperWindow = std::make_unique<WallpaperWindow>();
    auto power = std::make_unique<PowerManager>();
    auto themes = std::make_unique<Themes::ThemeManager>();
    auto particles = std::make_unique<Shape::Particles::AmbientParticleSystem>();
    auto audio = std::make_unique<Shape::Audio::AmbientPlayer>();
    auto bgRenderer = std::make_unique<Renderer::BackgroundRenderer>();
    auto particleRenderer = std::make_unique<Renderer::ParticleRenderer>();
    auto settingsWindow = std::make_unique<UI::SettingsWindow>();
    auto diary = std::make_unique<Diary::EcosystemDiary>();
    auto gameView = std::make_unique<GameView::GameViewWindow>();
    auto workshop = std::make_unique<Steam::WorkshopThemeManager>();

    bgRenderer->initialize();
    particleRenderer->initialize();
    diary->initialize("Content/_user/diary.json");
    workshop->initialize("Content/Themes/_workshop");
    GameView::SharedSimulationState::get().initialize();
    gameView->initialize(&GameView::SharedSimulationState::get());

    UI::SettingsState::get().load("Content/_user/settings.json");

    if (UI::SettingsState::get().autoStartWithSystem && !AutoStartManager::isEnabled()) {
        AutoStartManager::enable("PolygonalPrimordialsWallpaper");
    }

    auto themeApplier = std::make_unique<Themes::ThemeApplier>(
        &UI::SettingsState::get(), bgRenderer.get(),
        particleRenderer.get(), particles.get());
    themeApplier->initialize();

    WallpaperWindow::Mode wpMode = WallpaperWindow::Mode::BehindDesktop;
#ifndef _WIN32
    wpMode = WallpaperWindow::Mode::FullscreenAtBottom;
#endif

    if (!wallpaperWindow->initialize(wpMode, screenW, screenH)) {
        wallpaperWindow->initialize(WallpaperWindow::Mode::WindowedNormal, screenW, screenH);
    }
    wallpaperWindow->setClickThrough(true);

    settingsWindow->initialize(wallpaperWindow->getSDLWindow(), wallpaperWindow->getGLContext(), themes.get());

    PowerManager::Config pcfg;
    pcfg.fpsOnAC = 30;
    pcfg.fpsOnBattery = 15;
    pcfg.fpsWhenCovered = 5;
    power->initialize(pcfg);

    themes->initialize("Content/Themes");
    auto themeList = themes->discoverThemes();

    Themes::Theme currentTheme;
    if (!themeList.empty()) {
        currentTheme = themeList[0];
        themes->applyTheme(currentTheme);
    }

    particles->Initialize(3000);
    audio->Initialize();
    if (!currentTheme.ambientAudioPath.empty()) {
        audio->PlayThemeAudio(currentTheme.ambientAudioPath, currentTheme.audioVolume);
    }

    SystemTray::TrayConfig tcfg;
    tcfg.appName = "Polygonal Primordials Wallpaper";
    tcfg.tooltip = "Polygonal Primordials Wallpaper - Running";
    tcfg.iconPath = "assets/icons/tray.png";
    tray->initialize(tcfg);

    std::vector<std::pair<std::string, std::string>> themePairs;
    for (const auto& t : themeList) {
        themePairs.push_back({ t.id, t.displayName });
    }
    tray->setThemes(themePairs, currentTheme.id);

    tray->showNotification(
        "Polygonal Primordials Wallpaper",
        "Your wallpaper is now active. Right-click tray icon for options.",
        5000
    );

    tray->setMenuCallback([&](int itemId) {
        if (itemId == SystemTray::MENU_SETTINGS) {
            settingsWindow->show();
        }
        else if (itemId == SystemTray::MENU_OPEN_GAME) {
            settingsWindow->hide();
            gameView->show();
        }
        else if (itemId == SystemTray::MENU_PAUSE_TOGGLE) {
            power->setUserPaused(!power->isUserPaused());
            tray->setPaused(power->isUserPaused());
            tray->setTooltip(power->isUserPaused() ? 
                "Polygonal Primordials - Paused" :
                "Polygonal Primordials - Running");
        }
        else if (itemId >= SystemTray::MENU_THEME_BASE && 
                 itemId < SystemTray::MENU_THEME_BASE + (int)themeList.size()) {
            int themeIdx = itemId - SystemTray::MENU_THEME_BASE;
            auto newTheme = themes->loadTheme(themeList[themeIdx].id);
            if (newTheme) {
                themes->startTransition(currentTheme, *newTheme, 5.0f);
                currentTheme = *newTheme;
                tray->setThemes(themePairs, currentTheme.id);
                audio->PlayThemeAudio(currentTheme.ambientAudioPath, currentTheme.audioVolume, 5.0f);
                diary->notifyThemeChanged(currentTheme.displayName, newTheme->displayName);
                tray->showNotification("Theme changed", currentTheme.displayName);
            }
        }
        else if (itemId == SystemTray::MENU_QUIT) {
            SDL_Event quit;
            quit.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quit);
        }
    });

    bool running = true;
    auto lastTime = std::chrono::steady_clock::now();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN || 
                event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                event.type == SDL_EVENT_MOUSE_MOTION) {
                power->notifyUserInput();
            }
        }

        tray->poll();
        if (tray->shouldQuit()) running = false;

        auto now = std::chrono::steady_clock::now();
        double frameDt = std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        float dtF32 = static_cast<float>(frameDt);
        power->update(dtF32);
        themes->update(dtF32);
        audio->Update(dtF32);
        settingsWindow->update(dtF32);
        gameView->update(dtF32);
        workshop->update();

        WallpaperWindow::RenderStatus rs = wallpaperWindow->getStatus();
        bool shouldRender = (rs != WallpaperWindow::RenderStatus::ObscuredByFullscreen) &&
                             (rs != WallpaperWindow::RenderStatus::Hidden);
        shouldRender &= power->shouldRender();

        particles->Update(dtF32, static_cast<float>(screenW), static_cast<float>(screenH), currentTheme.accentColor, currentTheme.particleDensity);

        if (shouldRender) {
            Themes::Theme effective = themeApplier->getEffectiveTheme();
            bgRenderer->render(screenW, screenH, effective, static_cast<float>(SDL_GetTicks()) / 1000.0f);
            if (UI::SettingsState::get().particlesEnabled) {
                particleRenderer->render(particles.get(), effective, static_cast<float>(screenW), static_cast<float>(screenH));
            }
            settingsWindow->render();
            gameView->render();
            wallpaperWindow->present();
        }

        int targetFPS = power->getTargetFPS();
        double targetFrameTime = 1.0 / targetFPS;
        double actualFrameTime = std::chrono::duration<double>(
            std::chrono::steady_clock::now() - now).count();
        if (actualFrameTime < targetFrameTime) {
            SDL_Delay((Uint32)((targetFrameTime - actualFrameTime) * 1000));
        }
    }

    gameView->shutdown();
    workshop->shutdown();
    diary->shutdown();
    settingsWindow->shutdown();
    themeApplier->shutdown();
    particleRenderer->shutdown();
    bgRenderer->shutdown();
    audio->Shutdown();
    tray->shutdown();
    themes->shutdown();
    power->shutdown();
    wallpaperWindow->shutdown();
    SDL_Quit();

    return 0;
}
