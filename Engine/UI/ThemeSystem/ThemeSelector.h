// Engine/UI/ThemeSystem/ThemeSelector.h
#pragma once

#include "Themes/Theme.h"
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>

namespace ShapeEngine::UI {

    struct UIThemeColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        UIThemeColorRGBA() = default;
        UIThemeColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// Theme selection UI with live preview. Shows all installed themes
    /// with thumbnails, descriptions, and lets user preview before selecting.
    /// </summary>
    class ThemeSelector {
    public:
        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            std::string themesDirectory = "Content/Themes";
            
            // Layout
            int panelWidth = 700;
            int panelHeight = 600;
            int thumbnailSize = 180;
            int thumbnailsPerRow = 3;
            int thumbnailSpacing = 20;
            int scrollbarWidth = 12;
            
            // Styling
            UIThemeColorRGBA backgroundColor = {15, 18, 25, 245};
            UIThemeColorRGBA panelColor = {25, 28, 35, 250};
            UIThemeColorRGBA borderColor = {60, 80, 110, 255};
            UIThemeColorRGBA hoverBorderColor = {100, 180, 255, 255};
            UIThemeColorRGBA selectedBorderColor = {80, 200, 120, 255};
            UIThemeColorRGBA textColor = {230, 235, 245, 255};
            UIThemeColorRGBA secondaryTextColor = {140, 150, 170, 255};
            UIThemeColorRGBA accentColor = {100, 180, 255, 255};
            
            // Header
            std::string title = "Choose Your Theme";
            std::string subtitle = "Each theme changes the visual style of your ecosystem";
            int titleFontSize = 32;
            int subtitleFontSize = 14;
            
            // Buttons
            std::string applyButtonText = "Apply Theme";
            std::string closeButtonText = "Close";
            std::string workshopButtonText = "Browse Workshop";
            
            // Behavior
            bool showPreviewBeforeApply = true;
            float previewDuration = 5.0f;  // Seconds
            bool allowLivePreview = true;
        };

        explicit ThemeSelector(const Config& config);
        ~ThemeSelector();

        // ─── Display ─────────────────────────────────────────
        void render();
        void update(float deltaTime);
        bool handleInput();

        // ─── Visibility ─────────────────────────────────────
        void show() { m_visible = true; m_firstRender = true; }
        void hide() { m_visible = false; }
        void toggle() { m_visible = !m_visible; m_firstRender = true; }
        bool isVisible() const { return m_visible; }

        // ─── Theme Selection ───────────────────────────────
        void setSelectedTheme(const std::string& themeId);
        std::string getSelectedTheme() const { return m_selectedThemeId; }
        
        // ─── Callbacks ───────────────────────────────────────
        using ThemeCallback = std::function<void(const std::string& themeId)>;
        void setOnThemeSelected(ThemeCallback cb) { m_themeCallback = std::move(cb); }
        using CloseCallback = std::function<void()>;
        void setOnClose(CloseCallback cb) { m_closeCallback = std::move(cb); }
        using WorkshopCallback = std::function<void()>;
        void setOnOpenWorkshop(WorkshopCallback cb) { m_workshopCallback = std::move(cb); }

        // ─── Theme Loading ──────────────────────────────────
        void refreshThemes();
        const std::vector<Themes::Theme>& getAvailableThemes() const { return m_themes; }

    private:
        struct ThemeEntry {
            Themes::Theme theme;
            SDL_Texture* thumbnail = nullptr;
            SDL_Texture* previewTexture = nullptr;
            SDL_Rect cardRect{0, 0, 0, 0};
            bool isHovered = false;
            bool isSelected = false;
        };

        void renderHeader();
        void renderThemeGrid();
        void renderThemeCard(ThemeEntry& entry);
        void renderFooter();
        void renderPreviewOverlay();
        void renderCloseButton();
        void renderWorkshopButton();
        
        void renderText(const std::string& text, int x, int y, int size, 
                         UIThemeColorRGBA color, bool centered = false);
        SDL_Texture* generateThumbnail(const Themes::Theme& theme);
        SDL_Texture* loadTexture(const std::string& path);
        std::string formatThemeDescription(const Themes::Theme& theme);
        int panelY_start() const;

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        
        bool m_visible = false;
        bool m_firstRender = true;
        
        std::vector<Themes::Theme> m_themes;
        std::vector<ThemeEntry> m_themeEntries;
        std::string m_selectedThemeId;
        std::string m_currentThemeId;
        
        // Preview
        bool m_showPreview = false;
        float m_previewTimer = 0.0f;
        std::string m_previewingThemeId;
        
        // Scroll
        int m_scrollOffset = 0;
        int m_maxScroll = 0;
        
        // Callbacks
        ThemeCallback m_themeCallback;
        CloseCallback m_closeCallback;
        WorkshopCallback m_workshopCallback;
    };

} // namespace ShapeEngine::UI
