// Engine/UI/Achievements/AchievementPanel.cpp
#include "AchievementPanel.h"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <cstdio>
#include <ctime>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    AchievementPanel::AchievementPanel(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
    }

    AchievementPanel::~AchievementPanel() = default;

    void AchievementPanel::loadAchievements(const std::vector<Achievement>& achievements) {
        m_achievements = achievements;
        m_totalCount = (int)m_achievements.size();
        m_unlockedCount = 0;
        m_completionPercent = 0.0f;
        
        for (const auto& ach : m_achievements) {
            if (ach.unlocked) m_unlockedCount++;
        }
        if (m_totalCount > 0) {
            m_completionPercent = (float)m_unlockedCount / m_totalCount * 100.0f;
        }
        
        int rows = (m_totalCount + m_config.columns - 1) / m_config.columns;
        int totalHeight = rows * (m_config.cellSize + m_config.cellSpacing);
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        m_maxScroll = std::max(0, totalHeight - (winH - 150));
    }

    void AchievementPanel::unlockAchievement(const std::string& id) {
        for (auto& ach : m_achievements) {
            if (ach.id == id && !ach.unlocked) {
                ach.unlocked = true;
                ach.progress = 1.0f;
                ach.unlockTimestamp = std::time(nullptr);
                m_unlockedCount++;
                if (m_totalCount > 0) {
                    m_completionPercent = (float)m_unlockedCount / m_totalCount * 100.0f;
                }
                
                SHAPE_LOG_INFO("Achievement unlocked: %s", ach.name.c_str());
                if (m_onUnlock) m_onUnlock(ach);
                break;
            }
        }
    }

    void AchievementPanel::setProgress(const std::string& id, float progress) {
        for (auto& ach : m_achievements) {
            if (ach.id == id) {
                ach.progress = std::clamp(progress, 0.0f, 1.0f);
                break;
            }
        }
    }

    const AchievementPanel::Achievement* 
        AchievementPanel::getAchievement(const std::string& id) const {
        for (const auto& ach : m_achievements) {
            if (ach.id == id) return &ach;
        }
        return nullptr;
    }

    void AchievementPanel::update(float deltaTime) {
        (void)deltaTime;
    }

    bool AchievementPanel::handleInput() {
        if (!m_visible) return false;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    hide();
                    return true;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                m_scrollOffset -= (int)(event.wheel.y * 30.0f);
                m_scrollOffset = std::clamp(m_scrollOffset, 0, m_maxScroll);
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = (int)event.button.x;
                int my = (int)event.button.y;
                int winW = 1280, winH = 720;
                GetWindowDims(m_window, winW, winH);
                
                if (mx > winW - 60 && my < 50) {
                    hide();
                    return true;
                }
                
                for (size_t i = 0; i < m_achievements.size(); ++i) {
                    int col = (int)i % m_config.columns;
                    int row = (int)i / m_config.columns;
                    int cellX = 50 + col * (m_config.cellSize + m_config.cellSpacing);
                    int cellY = 150 + row * (m_config.cellSize + m_config.cellSpacing) - m_scrollOffset;
                    
                    if (mx >= cellX && mx <= cellX + m_config.cellSize &&
                        my >= cellY && my <= cellY + m_config.cellSize) {
                        m_selectedIndex = (int)i;
                        break;
                    }
                }
            }
        }
        return true;
    }

    void AchievementPanel::render() {
        if (!m_visible || !m_renderer) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, m_config.backgroundColor.r,
                                m_config.backgroundColor.g,
                                m_config.backgroundColor.b, m_config.backgroundColor.a);
        SDL_FRect bg = {0.0f, 0.0f, (float)winW, (float)winH};
        SDL_RenderFillRect(m_renderer, &bg);
        
        renderText("Achievements", 50, 30, 24, m_config.textColor);
        
        char stats[64];
        snprintf(stats, sizeof(stats), "%.0f%% Complete (%d / %d)",
                 m_completionPercent, m_unlockedCount, m_totalCount);
        renderText(stats, 50, 70, 14, m_config.accentColor);
        
        SDL_FRect progressBg = {50.0f, 95.0f, 400.0f, 12.0f};
        SDL_SetRenderDrawColor(m_renderer, 40, 45, 55, 255);
        SDL_RenderFillRect(m_renderer, &progressBg);
        
        float fillW = 400.0f * (m_completionPercent / 100.0f);
        SDL_FRect progressFill = {50.0f, 95.0f, fillW, 12.0f};
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, m_config.accentColor.b, 255);
        SDL_RenderFillRect(m_renderer, &progressFill);
        
        renderCloseButton();
        renderAchievementGrid();
        
        if (m_selectedIndex >= 0 && m_selectedIndex < (int)m_achievements.size()) {
            renderAchievementDetails(m_achievements[m_selectedIndex]);
        }
    }

    void AchievementPanel::renderAchievementGrid() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);
        m_hoveredIndex = -1;
        
        int gridY = 150;
        int gridHeight = winH - gridY - 50;
        
        for (size_t i = 0; i < m_achievements.size(); ++i) {
            int col = (int)i % m_config.columns;
            int row = (int)i / m_config.columns;
            int cellX = 50 + col * (m_config.cellSize + m_config.cellSpacing);
            int cellY = gridY + row * (m_config.cellSize + m_config.cellSpacing) - m_scrollOffset;
            
            if (cellY + m_config.cellSize < gridY || cellY > gridY + gridHeight) continue;
            
            if (mouseX >= cellX && mouseX <= cellX + m_config.cellSize &&
                mouseY >= cellY && mouseY <= cellY + m_config.cellSize) {
                m_hoveredIndex = (int)i;
            }
            
            renderAchievementCell(m_achievements[i], (int)i);
        }
    }

    void AchievementPanel::renderAchievementCell(const Achievement& ach, int index) {
        int col = index % m_config.columns;
        int row = index / m_config.columns;
        int cellX = 50 + col * (m_config.cellSize + m_config.cellSpacing);
        int cellY = 150 + row * (m_config.cellSize + m_config.cellSpacing) - m_scrollOffset;
        
        bool hovered = (m_hoveredIndex == index);
        bool selected = (m_selectedIndex == index);
        bool unlocked = ach.unlocked;
        
        SDL_FRect cellRect = {(float)cellX, (float)cellY, (float)m_config.cellSize, (float)m_config.cellSize};
        
        AchievementColorRGBA bgColor = unlocked ? m_config.cellUnlockedColor :
                                        hovered ? m_config.cellHoverColor : m_config.cellColor;
        
        if (!unlocked) {
            bgColor.r = (Uint8)(bgColor.r * 0.5f);
            bgColor.g = (Uint8)(bgColor.g * 0.5f);
            bgColor.b = (Uint8)(bgColor.b * 0.5f);
        }
        
        SDL_SetRenderDrawColor(m_renderer, bgColor.r, bgColor.g, bgColor.b, 255);
        SDL_RenderFillRect(m_renderer, &cellRect);
        
        AchievementColorRGBA borderColor = selected ? m_config.accentColor :
                                            unlocked ? m_config.goldColor : m_config.borderColor;
        SDL_SetRenderDrawColor(m_renderer, borderColor.r, borderColor.g, borderColor.b, 255);
        SDL_RenderRect(m_renderer, &cellRect);
        
        SDL_FRect iconRect = {(float)(cellX + 10), (float)(cellY + 10), 
                              (float)(m_config.cellSize - 20), (float)(m_config.cellSize - 20)};
        if (unlocked) {
            SDL_SetRenderDrawColor(m_renderer, m_config.goldColor.r, m_config.goldColor.g, m_config.goldColor.b, 200);
            SDL_RenderFillRect(m_renderer, &iconRect);
            renderText("[*]", cellX + m_config.cellSize / 2, cellY + m_config.cellSize / 2 - 8, 16, {40, 30, 0, 255}, true);
        } else {
            SDL_SetRenderDrawColor(m_renderer, 30, 30, 40, 200);
            SDL_RenderFillRect(m_renderer, &iconRect);
            renderText("?", cellX + m_config.cellSize / 2, cellY + m_config.cellSize / 2 - 8, 16, {120, 120, 130, 255}, true);
        }
        
        if (!unlocked && ach.progress > 0.0f) {
            SDL_FRect progBg = {(float)cellX, (float)(cellY + m_config.cellSize - 6), (float)m_config.cellSize, 6.0f};
            SDL_SetRenderDrawColor(m_renderer, 30, 30, 40, 255);
            SDL_RenderFillRect(m_renderer, &progBg);
            
            SDL_FRect progFill = {(float)cellX, (float)(cellY + m_config.cellSize - 6), (float)(m_config.cellSize * ach.progress), 6.0f};
            SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r, m_config.accentColor.g, m_config.accentColor.b, 255);
            SDL_RenderFillRect(m_renderer, &progFill);
        }
    }

    void AchievementPanel::renderAchievementDetails(const Achievement& ach) {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int panelX = winW - 400;
        int panelY = 150;
        int panelW = 350;
        int panelH = winH - panelY - 100;
        
        SDL_FRect panel = {(float)panelX, (float)panelY, (float)panelW, (float)panelH};
        SDL_SetRenderDrawColor(m_renderer, 25, 28, 35, 245);
        SDL_RenderFillRect(m_renderer, &panel);
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r, m_config.accentColor.g, m_config.accentColor.b, 200);
        SDL_RenderRect(m_renderer, &panel);
        
        SDL_FRect iconRect = {(float)(panelX + 125), (float)(panelY + 20), 100.0f, 100.0f};
        if (ach.unlocked) {
            SDL_SetRenderDrawColor(m_renderer, m_config.goldColor.r, m_config.goldColor.g, m_config.goldColor.b, 255);
            SDL_RenderFillRect(m_renderer, &iconRect);
            renderText("[*]", panelX + 175, panelY + 55, 20, {40, 30, 0, 255}, true);
        } else {
            SDL_SetRenderDrawColor(m_renderer, 40, 40, 50, 255);
            SDL_RenderFillRect(m_renderer, &iconRect);
            renderText("?", panelX + 175, panelY + 55, 20, {100, 100, 110, 255}, true);
        }
        
        renderText(ach.unlocked ? ach.name : "??? Hidden ???", 
                   panelX + 175, panelY + 140, 16, 
                   ach.unlocked ? m_config.goldColor : m_config.secondaryTextColor, true);
        
        renderText(ach.unlocked ? "UNLOCKED" : "LOCKED", 
                   panelX + 175, panelY + 165, 12, 
                   ach.unlocked ? m_config.goldColor : m_config.textColor, true);
        
        int descY = panelY + 200;
        std::string descText = ach.unlocked ? ach.description : 
                               (ach.isHidden ? "Hidden achievement" : ach.description);
        renderText(descText, panelX + 20, descY, 12, m_config.textColor);
        
        if (!ach.unlocked && ach.progress > 0) {
            char progStr[64];
            snprintf(progStr, sizeof(progStr), "Progress: %.0f%%", ach.progress * 100.0f);
            renderText(progStr, panelX + 20, descY + 30, 11, m_config.secondaryTextColor);
            
            SDL_FRect progBg = {(float)(panelX + 20), (float)(descY + 50), (float)(panelW - 40), 8.0f};
            SDL_SetRenderDrawColor(m_renderer, 40, 45, 55, 255);
            SDL_RenderFillRect(m_renderer, &progBg);
            
            SDL_FRect progFill = {(float)(panelX + 20), (float)(descY + 50), (float)((panelW - 40) * ach.progress), 8.0f};
            SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r, m_config.accentColor.g, m_config.accentColor.b, 255);
            SDL_RenderFillRect(m_renderer, &progFill);
        }
        
        if (ach.unlocked && ach.unlockTimestamp > 0) {
            char dateStr[64];
            time_t ts = (time_t)ach.unlockTimestamp;
            std::tm tm_buf;
        #ifdef _WIN32
            localtime_s(&tm_buf, &ts);
        #else
            localtime_r(&ts, &tm_buf);
        #endif
            std::strftime(dateStr, sizeof(dateStr), "Unlocked: %Y-%m-%d %H:%M", &tm_buf);
            renderText(dateStr, panelX + 20, panelY + panelH - 40, 10, m_config.secondaryTextColor);
        }
    }

    void AchievementPanel::renderCloseButton() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_FRect closeBtn = {(float)(winW - 40), 20.0f, 25.0f, 25.0f};
        SDL_SetRenderDrawColor(m_renderer, 100, 60, 60, 200);
        SDL_RenderFillRect(m_renderer, &closeBtn);
        renderText("X", winW - 27, 24, 14, {240, 240, 240, 255}, true);
    }

    void AchievementPanel::renderText(const std::string& text, int x, int y, int size, 
                                        AchievementColorRGBA color, bool centered) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        float drawX = centered ? (float)x - (text.length() * 4.0f) : (float)x;
        SDL_RenderDebugText(m_renderer, drawX, (float)y, text.c_str());
    }

} // namespace ShapeEngine::UI
