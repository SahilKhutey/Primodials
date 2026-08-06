// Engine/GameView/GameViewWindow.cpp
#include "GameViewWindow.h"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>
#include <cstdio>
#include <cmath>

namespace ShapeEngine::GameView {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    GameViewWindow::GameViewWindow(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
    }

    GameViewWindow::~GameViewWindow() = default;

    void GameViewWindow::show() {
        if (m_window) {
            SDL_ShowWindow(m_window);
        }
        m_visible = true;
    }

    void GameViewWindow::hide() {
        if (m_window) {
            SDL_HideWindow(m_window);
        }
        m_visible = false;
    }

    void GameViewWindow::toggle() {
        if (m_visible) hide();
        else show();
    }

    void GameViewWindow::setPaused(bool paused) {
        m_paused = paused;
    }

    void GameViewWindow::update(float deltaTime) {
        (void)deltaTime;
    }

    GameViewAction GameViewWindow::handleInput() {
        if (!m_visible) return GameViewAction::None;
        
        GameViewAction action = GameViewAction::None;
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && 
                m_window && event.window.windowID == SDL_GetWindowID(m_window)) {
                hide();
                return GameViewAction::Close;
            }
            
            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_SPACE:
                        action = GameViewAction::PauseToggle;
                        break;
                    case SDLK_RIGHTBRACKET:
                        action = GameViewAction::SpeedUp;
                        break;
                    case SDLK_LEFTBRACKET:
                        action = GameViewAction::SpeedDown;
                        break;
                    case SDLK_F5:
                        action = GameViewAction::SaveSnapshot;
                        break;
                    case SDLK_F9:
                        action = GameViewAction::LoadSnapshot;
                        break;
                    case SDLK_TAB:
                        action = GameViewAction::OpenInspector;
                        break;
                    case SDLK_F2:
                        action = GameViewAction::OpenThemeSelector;
                        break;
                    case SDLK_ESCAPE:
                        action = GameViewAction::Close;
                        break;
                }
            }
            
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = (int)event.button.x;
                int my = (int)event.button.y;
                
                if (my < 40) {
                    if (mx >= 20 && mx <= 60) action = GameViewAction::PauseToggle;
                    else if (mx >= 70 && mx <= 110) action = GameViewAction::SpeedDown;
                    else if (mx >= 120 && mx <= 160) action = GameViewAction::SpeedUp;
                    else if (mx >= 170 && mx <= 210) action = GameViewAction::SaveSnapshot;
                    else if (mx >= 220 && mx <= 260) action = GameViewAction::LoadSnapshot;
                }
                
                if (my < 40 && mx > winW - 200) {
                    if (mx >= winW - 190 && mx <= winW - 150) action = GameViewAction::OpenThemeSelector;
                    else if (mx >= winW - 140 && mx <= winW - 100) action = GameViewAction::OpenSettings;
                    else if (mx >= winW - 90 && mx <= winW - 50) action = GameViewAction::Close;
                }
            }
        }
        
        if (action != GameViewAction::None && m_actionCb) {
            m_actionCb(action);
        }
        
        return action;
    }

    void GameViewWindow::render() {
        if (!m_visible || !m_renderer) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_SetRenderDrawColor(m_renderer, 12, 14, 18, 255);
        SDL_FRect bg = {0.0f, 0.0f, (float)winW, (float)winH};
        SDL_RenderFillRect(m_renderer, &bg);
        
        renderTopBar();
        renderSimulationViewport();
        renderBottomBar();
        
        if (m_showSidePanel) {
            renderSidePanel();
        }
    }

    void GameViewWindow::renderTopBar() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int barH = 40;
        
        SDL_SetRenderDrawColor(m_renderer, m_config.topBarColor.r,
                                m_config.topBarColor.g,
                                m_config.topBarColor.b, 255);
        SDL_FRect bar = {0.0f, 0.0f, (float)winW, (float)barH};
        SDL_RenderFillRect(m_renderer, &bar);
        
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g,
                                m_config.accentColor.b, 100);
        SDL_RenderLine(m_renderer, 0.0f, (float)barH, (float)winW, (float)barH);
        
        renderText(m_config.windowTitle.c_str(), 10, 12, 12, m_config.textColor);
        
        renderToolButton(20, 5, 35, 30, m_paused ? "P" : "||", 
                          m_paused ? "Resume (Space)" : "Pause (Space)", 
                          m_paused);
        renderToolButton(65, 5, 35, 30, "<<", "Speed Down ([)", false);
        renderToolButton(110, 5, 35, 30, ">>", "Speed Up (])", false);
        renderToolButton(160, 5, 35, 30, "S", "Save Snapshot (F5)", false);
        renderToolButton(200, 5, 35, 30, "L", "Load Snapshot (F9)", false);
        
        int rightX = winW - 200;
        renderToolButton(rightX, 5, 35, 30, "T", "Themes (F2)", false);
        renderToolButton(rightX + 50, 5, 35, 30, "Cfg", "Settings", false);
        renderToolButton(rightX + 100, 5, 35, 30, "X", "Close (ESC)", false);
    }

    void GameViewWindow::renderSimulationViewport() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int topBarH = 40;
        int bottomBarH = 50;
        int sidePanelW = m_showSidePanel ? 300 : 0;
        
        SDL_Rect viewport = {
            sidePanelW, topBarH,
            winW - sidePanelW, 
            winH - topBarH - bottomBarH
        };
        
        int cx = viewport.x + viewport.w / 2;
        int cy = viewport.y + viewport.h / 2;
        
        SDL_SetRenderDrawColor(m_renderer, 100, 180, 255, 100);
        SDL_RenderLine(m_renderer, (float)(cx - 10), (float)cy, (float)(cx + 10), (float)cy);
        SDL_RenderLine(m_renderer, (float)cx, (float)(cy - 10), (float)cx, (float)(cy + 10));
        
        renderText("(Simulation Viewport)", cx - 60, cy + 20, 12, 
                   {150, 150, 150, 150});
        
        renderText("Click to select entities * WASD to pan * Wheel to zoom",
                   viewport.x + 10, viewport.y + viewport.h - 25, 11, 
                   {200, 200, 210, 200});
    }

    void GameViewWindow::renderBottomBar() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int barH = 50;
        int barY = winH - barH;
        
        SDL_SetRenderDrawColor(m_renderer, m_config.bottomBarColor.r,
                                m_config.bottomBarColor.g,
                                m_config.bottomBarColor.b, 255);
        SDL_FRect bar = {0.0f, (float)barY, (float)winW, (float)barH};
        SDL_RenderFillRect(m_renderer, &bar);
        
        char tickText[64];
        snprintf(tickText, sizeof(tickText), "Tick: %llu", 
                 (unsigned long long)m_tickCount);
        renderText(tickText, 10, barY + 18, 13, m_config.textColor);
        
        char entityText[64];
        snprintf(entityText, sizeof(entityText), "Entities: %d", m_entityCount);
        renderText(entityText, 150, barY + 18, 13, m_config.textColor);
        
        char fpsText[64];
        snprintf(fpsText, sizeof(fpsText), "FPS: %.1f", m_fps);
        GameViewColorRGBA fpsColor = (m_fps >= 30) ? m_config.textColor : m_config.warningColor;
        renderText(fpsText, 290, barY + 18, 13, fpsColor);
        
        char speedText[64];
        snprintf(speedText, sizeof(speedText), "Speed: %.2fx", m_simSpeed);
        renderText(speedText, 400, barY + 18, 13, m_config.textColor);
        
        char themeText[128];
        snprintf(themeText, sizeof(themeText), "Theme: %s", m_themeName.c_str());
        renderText(themeText, 520, barY + 18, 13, m_config.accentColor);
        
        renderText("F1-F12: Themes | F5: Save | F9: Load | TAB: Inspector | ESC: Close",
                   winW - 460, barY + 18, 11, {180, 180, 190, 255});
    }

    void GameViewWindow::renderSidePanel() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int panelW = 300;
        int panelY = 40;
        int panelH = winH - 40 - 50;
        
        SDL_FRect panel = {0.0f, (float)panelY, (float)panelW, (float)panelH};
        SDL_SetRenderDrawColor(m_renderer, 20, 22, 28, 240);
        SDL_RenderFillRect(m_renderer, &panel);
        
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g,
                                m_config.accentColor.b, 80);
        SDL_RenderLine(m_renderer, (float)panelW, (float)panelY, (float)panelW, (float)(panelY + panelH));
        
        renderText("SIMULATION CONTROL", 15, panelY + 15, 13, m_config.accentColor);
        
        int infoY = panelY + 50;
        renderText("Population:", 15, infoY, 11, {180, 180, 190, 255});
        infoY += 20;
        
        const char* speciesNames[] = {"Species 0", "Species 1", "Species 2"};
        int speciesCounts[] = {1234, 567, 89};
        GameViewColorRGBA speciesColors[] = {
            {100, 180, 255, 255},
            {255, 150, 100, 255},
            {150, 255, 100, 255}
        };
        
        for (int i = 0; i < 3; ++i) {
            renderText(speciesNames[i], 15, infoY, 11, {200, 200, 210, 255});
            
            SDL_FRect bar = {100.0f, (float)(infoY + 2), 180.0f, 10.0f};
            SDL_SetRenderDrawColor(m_renderer, 40, 45, 55, 255);
            SDL_RenderFillRect(m_renderer, &bar);
            
            float pct = (float)speciesCounts[i] / 1500.0f;
            SDL_FRect barFill = {100.0f, (float)(infoY + 2), (float)(180 * pct), 10.0f};
            SDL_SetRenderDrawColor(m_renderer, speciesColors[i].r,
                                    speciesColors[i].g, speciesColors[i].b, 255);
            SDL_RenderFillRect(m_renderer, &barFill);
            
            char countText[16];
            snprintf(countText, sizeof(countText), "%d", speciesCounts[i]);
            renderText(countText, 285, infoY, 11, {200, 200, 210, 255});
            
            infoY += 18;
        }
        
        infoY += 20;
        renderText("QUICK ACTIONS:", 15, infoY, 13, m_config.accentColor);
        infoY += 20;
        
        SDL_FRect spawnBtn = {15.0f, (float)infoY, (float)(panelW - 30), 30.0f};
        SDL_SetRenderDrawColor(m_renderer, m_config.buttonColor.r,
                                m_config.buttonColor.g, 
                                m_config.buttonColor.b, 255);
        SDL_RenderFillRect(m_renderer, &spawnBtn);
        renderText("+ Spawn Random Entity", (int)(spawnBtn.x + spawnBtn.w / 2), 
                   (int)(spawnBtn.y + 8), 12, m_config.textColor, true);
        infoY += 40;
        
        SDL_FRect resetBtn = {15.0f, (float)infoY, (float)(panelW - 30), 30.0f};
        SDL_SetRenderDrawColor(m_renderer, m_config.dangerColor.r,
                                m_config.dangerColor.g, 
                                m_config.dangerColor.b, 255);
        SDL_RenderFillRect(m_renderer, &resetBtn);
        renderText("Reset Simulation", (int)(resetBtn.x + resetBtn.w / 2), 
                   (int)(resetBtn.y + 8), 12, m_config.textColor, true);
    }

    void GameViewWindow::renderToolButton(int x, int y, int w, int h, 
                                            const std::string& icon, 
                                            const std::string& tooltip, 
                                            bool active) {
        (void)tooltip;
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool hovered = (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
        
        GameViewColorRGBA bgColor = active ? m_config.accentColor : 
                            hovered ? m_config.buttonHoverColor : m_config.buttonColor;
        bgColor.a = 200;
        
        SDL_FRect btn = {(float)x, (float)y, (float)w, (float)h};
        SDL_SetRenderDrawColor(m_renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(m_renderer, &btn);
        
        renderText(icon, x + w / 2, y + (h / 2 - 6), 14, m_config.textColor, true);
    }

    void GameViewWindow::renderText(const std::string& text, int x, int y, int size, 
                                       GameViewColorRGBA color, bool centered) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        float drawX = centered ? (float)x - (text.length() * 4.0f) : (float)x;
        SDL_RenderDebugText(m_renderer, drawX, (float)y, text.c_str());
    }

} // namespace ShapeEngine::GameView
