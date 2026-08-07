// Engine/UI/Achievements/AchievementPanel.h
#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>
#include <cstdint>

namespace ShapeEngine::UI {

    struct AchievementColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        AchievementColorRGBA() = default;
        AchievementColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// Achievement tracking panel - shows progress toward all 
    /// achievements in a grid layout. Click to view details.
    /// </summary>
    class AchievementPanel {
    public:
        struct Achievement {
            std::string id;
            std::string name;
            std::string description;
            std::string iconUnlocked;
            std::string iconLocked;
            bool unlocked = false;
            float progress = 0.0f;  // 0.0 - 1.0
            int64_t unlockTimestamp = 0;
            bool isHidden = false;  // Hidden until unlocked
        };

        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            int columns = 5;
            int cellSize = 90;
            int cellSpacing = 15;
            
            AchievementColorRGBA backgroundColor = {15, 18, 25, 245};
            AchievementColorRGBA cellColor = {30, 35, 45, 255};
            AchievementColorRGBA cellHoverColor = {50, 60, 80, 255};
            AchievementColorRGBA cellUnlockedColor = {80, 130, 180, 255};
            AchievementColorRGBA borderColor = {60, 70, 85, 255};
            AchievementColorRGBA textColor = {230, 235, 245, 255};
            AchievementColorRGBA secondaryTextColor = {140, 150, 170, 255};
            AchievementColorRGBA accentColor = {100, 180, 255, 255};
            AchievementColorRGBA goldColor = {255, 215, 0, 255};
        };

        explicit AchievementPanel(const Config& config);
        ~AchievementPanel();

        void render();
        void update(float deltaTime);
        bool handleInput();

        void show() { m_visible = true; }
        void hide() { m_visible = false; }
        void toggle() { m_visible = !m_visible; }
        bool isVisible() const { return m_visible; }

        // ─── Achievement management ───────────────────────────
        void loadAchievements(const std::vector<Achievement>& achievements);
        void unlockAchievement(const std::string& id);
        void setProgress(const std::string& id, float progress);
        const Achievement* getAchievement(const std::string& id) const;

        int getUnlockedCount() const { return m_unlockedCount; }
        int getTotalCount() const { return m_totalCount; }
        float getCompletionPercent() const { return m_completionPercent; }

        // ─── Callbacks ───────────────────────────────────────
        using UnlockCallback = std::function<void(const Achievement&)>;
        void setOnUnlock(UnlockCallback cb) { m_onUnlock = std::move(cb); }

    private:
        void renderAchievementGrid();
        void renderAchievementCell(const Achievement& ach, int index);
        void renderAchievementDetails(const Achievement& ach);
        void renderCloseButton();
        void renderText(const std::string& text, int x, int y, int size, 
                         AchievementColorRGBA color, bool centered = false);

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_visible = false;
        
        std::vector<Achievement> m_achievements;
        int m_hoveredIndex = -1;
        int m_selectedIndex = -1;
        int m_scrollOffset = 0;
        int m_maxScroll = 0;
        
        int m_unlockedCount = 0;
        int m_totalCount = 0;
        float m_completionPercent = 0.0f;
        
        UnlockCallback m_onUnlock;
    };

} // namespace ShapeEngine::UI
