// Engine/UI/HUD/GameHUD.cpp
#include "GameHUD.h"
#include "Core/Logger.hpp"
#include <cstdio>
#include <algorithm>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    GameHUD::GameHUD(SDL_Window* window, SDL_Renderer* renderer)
        : m_window(window), m_renderer(renderer) {
    }

    GameHUD::~GameHUD() {
    }

    void GameHUD::update(float deltaTime) {
        for (auto& notif : m_notifications) {
            notif.timeRemaining -= deltaTime;
        }
        m_notifications.erase(
            std::remove_if(m_notifications.begin(), m_notifications.end(),
                [](const Notification& n) { return n.timeRemaining <= 0; }),
            m_notifications.end());
    }

    void GameHUD::render() {
        if (!m_visible || !m_renderer) return;
        renderTopBar();
        renderBottomBar();
        renderNotifications();
    }

    void GameHUD::renderTopBar() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, m_bgColor.r, m_bgColor.g, 
                                m_bgColor.b, m_bgColor.a);
        SDL_FRect bar = {0.0f, 0.0f, (float)winW, 40.0f};
        SDL_RenderFillRect(m_renderer, &bar);
        
        renderText("Polygonal Primordials", 20, 10, 18, m_accentColor);
        
        float x = (float)(winW - 400);
        renderText("Speed:", (int)x, 10, 14, m_textColor);
        
        char speedText[16];
        snprintf(speedText, sizeof(speedText), "%.2fx", m_simSpeed);
        SDL_Color speedColor = m_paused ? m_warningColor : m_textColor;
        renderText(speedText, (int)x + 60, 10, 16, speedColor);
        
        SDL_FRect slowBtn = {x + 110.0f, 8.0f, 24.0f, 24.0f};
        SDL_FRect fastBtn = {x + 140.0f, 8.0f, 24.0f, 24.0f};
        renderText("<-", (int)slowBtn.x + 4, (int)slowBtn.y + 2, 18, m_textColor);
        renderText("->", (int)fastBtn.x + 4, (int)fastBtn.y + 2, 18, m_textColor);
        
        SDL_FRect pauseBtn = {x + 180.0f, 8.0f, 60.0f, 24.0f};
        SDL_SetRenderDrawColor(m_renderer, m_buttonColor.r, m_buttonColor.g, 
                                m_buttonColor.b, m_buttonColor.a);
        SDL_RenderFillRect(m_renderer, &pauseBtn);
        renderText(m_paused ? "RESUME" : "PAUSE", (int)pauseBtn.x + 8, (int)pauseBtn.y + 3, 
                   14, m_textColor);
        
        SDL_FRect menuBtn = {x + 250.0f, 8.0f, 80.0f, 24.0f};
        SDL_SetRenderDrawColor(m_renderer, m_buttonColor.r, m_buttonColor.g, 
                                m_buttonColor.b, m_buttonColor.a);
        SDL_RenderFillRect(m_renderer, &menuBtn);
        renderText("MENU (ESC)", (int)menuBtn.x + 4, (int)menuBtn.y + 3, 13, m_textColor);
    }

    void GameHUD::renderBottomBar() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, m_bgColor.r, m_bgColor.g, 
                                m_bgColor.b, m_bgColor.a);
        SDL_FRect bar = {0.0f, (float)(winH - 50), (float)winW, 50.0f};
        SDL_RenderFillRect(m_renderer, &bar);
        
        char tickText[32];
        snprintf(tickText, sizeof(tickText), "Tick: %llu", 
                 (unsigned long long)m_currentTick);
        renderText(tickText, 20, winH - 35, 14, m_textColor);
        
        char entityText[32];
        snprintf(entityText, sizeof(entityText), "Entities: %d", m_entityCount);
        renderText(entityText, 200, winH - 35, 14, m_textColor);
        
        char fpsText[32];
        snprintf(fpsText, sizeof(fpsText), "FPS: %.1f", m_fps);
        SDL_Color fpsColor = (m_fps >= 30) ? m_textColor : m_warningColor;
        renderText(fpsText, 400, winH - 35, 14, fpsColor);
        
        renderText("WASD:Pan  Wheel:Zoom  Click:Select  ESC:Menu", 
                   winW - 380, winH - 35, 13, m_textColor);
    }

    void GameHUD::renderNotifications() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        float y = 60.0f;
        
        for (const auto& notif : m_notifications) {
            float alpha = std::min(1.0f, notif.timeRemaining);
            SDL_Color color = notif.color;
            color.a = (Uint8)(color.a * alpha);
            
            if (!m_renderer) continue;
            float textLen = (float)(notif.text.length() * 8);
            SDL_FRect bg = {(float)(winW - textLen - 30), y, 
                            textLen + 20.0f, 24.0f};
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 
                (Uint8)(150 * alpha));
            SDL_RenderFillRect(m_renderer, &bg);
            
            renderText(notif.text, (int)(winW - textLen - 20), (int)y + 4, 14, color);
            y += 35.0f;
        }
    }

    GameHUD::HUDAction GameHUD::handleInput() {
        if (!m_visible) return HUDAction::None;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_SPACE: return HUDAction::PauseToggle;
                    case SDLK_LEFTBRACKET: return HUDAction::SpeedDown;
                    case SDLK_RIGHTBRACKET: return HUDAction::SpeedUp;
                    case SDLK_TAB: return HUDAction::OpenInspector;
                    case SDLK_F1: 
                        m_showStats = !m_showStats;
                        return HUDAction::ShowStats;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int winW = 1280, winH = 720;
                GetWindowDims(m_window, winW, winH);

                int mouseX = (int)event.button.x;
                int mouseY = (int)event.button.y;
                
                if (mouseY < 40) {
                    float baseX = (float)(winW - 400);
                    
                    if (mouseX >= baseX + 180 && mouseX <= baseX + 240) {
                        return HUDAction::PauseToggle;
                    }
                    if (mouseX >= baseX + 110 && mouseX <= baseX + 134) {
                        return HUDAction::SpeedDown;
                    }
                    if (mouseX >= baseX + 140 && mouseX <= baseX + 164) {
                        return HUDAction::SpeedUp;
                    }
                    if (mouseX >= baseX + 250 && mouseX <= baseX + 330) {
                        return HUDAction::None;
                    }
                }
            }
        }
        return HUDAction::None;
    }

    void GameHUD::renderText(const std::string& text, int x, int y, int size, 
                                SDL_Color color) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDebugText(m_renderer, (float)x, (float)y, text.c_str());
    }

    void GameHUD::addNotification(const std::string& text, float duration, SDL_Color color) {
        Notification n;
        n.text = text;
        n.timeRemaining = duration;
        n.color = color;
        m_notifications.push_back(n);
    }

} // namespace ShapeEngine::UI
