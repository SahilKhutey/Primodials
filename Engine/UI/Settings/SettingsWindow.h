// Engine/UI/Settings/SettingsWindow.h
#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace ShapeEngine::UI {

    struct SettingsColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        SettingsColorRGBA() = default;
        SettingsColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// Complete settings window with tabs: Performance, Visual, Audio, 
    /// Gameplay, Controls, Mods, Account.
    /// </summary>
    class SettingsWindow {
    public:
        enum class SettingsTab {
            Performance,
            Visual,
            Audio,
            Gameplay,
            Controls,
            Mods,
            Account
        };

        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            int windowWidth = 900;
            int windowHeight = 650;
            
            SettingsColorRGBA backgroundColor = {18, 20, 26, 250};
            SettingsColorRGBA tabBarColor = {25, 28, 35, 255};
            SettingsColorRGBA activeTabColor = {50, 80, 120, 255};
            SettingsColorRGBA inactiveTabColor = {30, 35, 45, 255};
            SettingsColorRGBA textColor = {230, 235, 245, 255};
            SettingsColorRGBA secondaryTextColor = {140, 150, 170, 255};
            SettingsColorRGBA accentColor = {100, 180, 255, 255};
            SettingsColorRGBA warningColor = {255, 200, 100, 255};
            SettingsColorRGBA successColor = {130, 220, 150, 255};
        };

        explicit SettingsWindow(const Config& config);
        ~SettingsWindow();

        void render();
        void update(float deltaTime);
        bool handleInput();

        void show() { m_visible = true; }
        void hide() { m_visible = false; }
        void toggle() { m_visible = !m_visible; }
        bool isVisible() const { return m_visible; }

        SettingsTab getActiveTab() const { return m_activeTab; }
        void setActiveTab(SettingsTab tab) { m_activeTab = tab; }

        struct SettingValue {
            enum Type { Bool, Int, Float, String };
            Type type = Bool;
            bool boolVal = false;
            int intVal = 0;
            float floatVal = 0.0f;
            std::string stringVal;

            SettingValue() = default;
            SettingValue(Type t, bool b, int i = 0, float f = 0.0f, const std::string& s = "")
                : type(t), boolVal(b), intVal(i), floatVal(f), stringVal(s) {}
        };
        
        SettingValue get(const std::string& key) const;
        void set(const std::string& key, const SettingValue& value);
        bool has(const std::string& key) const;
        void reset();

        using SettingChangedCallback = std::function<void(const std::string& key)>;
        void setOnSettingChanged(SettingChangedCallback cb) { m_onChange = std::move(cb); }

        bool save(const std::string& path) const;
        bool load(const std::string& path);

    private:
        void renderTabBar();
        void renderPerformanceTab();
        void renderVisualTab();
        void renderAudioTab();
        void renderGameplayTab();
        void renderControlsTab();
        void renderModsTab();
        void renderAccountTab();
        
        bool renderToggle(const std::string& label, bool& value, 
                          const std::string& description = "");
        bool renderSliderInt(const std::string& label, int& value, 
                              int min, int max, const std::string& description = "");
        bool renderSliderFloat(const std::string& label, float& value, 
                                float min, float max, const std::string& description = "");
        bool renderDropdown(const std::string& label, int& selected, 
                            const std::vector<std::string>& options, 
                            const std::string& description = "");
        bool renderButton(const std::string& label, SDL_Rect rect);
        bool renderSection(const std::string& title, int& y);
        
        void renderText(const std::string& text, int x, int y, int size, 
                         SettingsColorRGBA color, bool centered = false);

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_visible = false;
        SettingsTab m_activeTab = SettingsTab::Performance;
        
        int m_tempY = 70;
        std::unordered_map<std::string, SettingValue> m_settings;
        SettingChangedCallback m_onChange;
    };

} // namespace ShapeEngine::UI
