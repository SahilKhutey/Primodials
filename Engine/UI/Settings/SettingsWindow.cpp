// Engine/UI/Settings/SettingsWindow.cpp
#include "SettingsWindow.h"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>
#include <fstream>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    SettingsWindow::SettingsWindow(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
        
        set("performance.target_fps", SettingValue{SettingValue::Int, false, 30});
        set("performance.entity_count", SettingValue{SettingValue::Int, false, 10000});
        set("performance.auto_pause_fullscreen", SettingValue{SettingValue::Bool, true});
        set("performance.respect_battery", SettingValue{SettingValue::Bool, true});
        set("performance.target_quality", SettingValue{SettingValue::String, false, 0, 0.0f, "High"});
        
        set("visual.glow_intensity", SettingValue{SettingValue::Float, false, 0, 0.8f});
        set("visual.particle_density", SettingValue{SettingValue::Float, false, 0, 0.5f});
        set("visual.motion_speed", SettingValue{SettingValue::Float, false, 0, 1.0f});
        set("visual.show_grid_overlay", SettingValue{SettingValue::Bool, false});
        set("visual.vignette_intensity", SettingValue{SettingValue::Float, false, 0, 0.4f});
        
        set("audio.enabled", SettingValue{SettingValue::Bool, true});
        set("audio.volume", SettingValue{SettingValue::Float, false, 0, 0.5f});
        set("audio.mute_on_focus_loss", SettingValue{SettingValue::Bool, true});
        
        set("gameplay.simulation_speed", SettingValue{SettingValue::Float, false, 0, 1.0f});
        set("gameplay.auto_pause_on_idle", SettingValue{SettingValue::Bool, true});
        set("gameplay.idle_threshold", SettingValue{SettingValue::Int, false, 300});
        set("gameplay.difficulty", SettingValue{SettingValue::String, false, 0, 0.0f, "Normal"});
        
        set("controls.pan_speed", SettingValue{SettingValue::Float, false, 0, 1.0f});
        set("controls.zoom_speed", SettingValue{SettingValue::Float, false, 0, 1.0f});
        set("controls.invert_zoom", SettingValue{SettingValue::Bool, false});
    }

    SettingsWindow::~SettingsWindow() = default;

    void SettingsWindow::update(float deltaTime) {
        (void)deltaTime;
    }

    bool SettingsWindow::handleInput() {
        if (!m_visible) return false;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                hide();
                return true;
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = (int)event.button.x;
                int mouseY = (int)event.button.y;
                
                if (mouseY < 50) {
                    int winW = 1280, winH = 720;
                    GetWindowDims(m_window, winW, winH);
                    int numTabs = 7;
                    int tabWidth = winW / numTabs;
                    int tabIdx = mouseX / tabWidth;
                    if (tabIdx >= 0 && tabIdx < numTabs) {
                        m_activeTab = static_cast<SettingsTab>(tabIdx);
                        return true;
                    }
                }
            }
        }
        return true;
    }

    void SettingsWindow::render() {
        if (!m_visible || !m_renderer) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);

        SDL_SetRenderDrawColor(m_renderer, m_config.backgroundColor.r,
                                m_config.backgroundColor.g,
                                m_config.backgroundColor.b, 250);
        SDL_FRect fullScreen = {0.0f, 0.0f, (float)winW, (float)winH};
        SDL_RenderFillRect(m_renderer, &fullScreen);
        
        SDL_SetRenderDrawColor(m_renderer, 100, 110, 130, 255);
        SDL_RenderRect(m_renderer, &fullScreen);
        
        renderTabBar();
        
        m_tempY = 70;
        switch (m_activeTab) {
            case SettingsTab::Performance: renderPerformanceTab(); break;
            case SettingsTab::Visual: renderVisualTab(); break;
            case SettingsTab::Audio: renderAudioTab(); break;
            case SettingsTab::Gameplay: renderGameplayTab(); break;
            case SettingsTab::Controls: renderControlsTab(); break;
            case SettingsTab::Mods: renderModsTab(); break;
            case SettingsTab::Account: renderAccountTab(); break;
        }
    }

    void SettingsWindow::renderTabBar() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int tabHeight = 50;
        
        SDL_SetRenderDrawColor(m_renderer, m_config.tabBarColor.r,
                                m_config.tabBarColor.g,
                                m_config.tabBarColor.b, 255);
        SDL_FRect tabBar = {0.0f, 0.0f, (float)winW, (float)tabHeight};
        SDL_RenderFillRect(m_renderer, &tabBar);
        
        const char* tabNames[] = {
            "Performance", "Visual", "Audio", "Gameplay", 
            "Controls", "Mods", "Account"
        };
        int numTabs = 7;
        int tabWidth = winW / numTabs;
        
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        for (int i = 0; i < numTabs; ++i) {
            SDL_FRect tabRect = {(float)(i * tabWidth), 0.0f, (float)tabWidth, (float)tabHeight};
            
            bool isActive = ((int)m_activeTab == i);
            bool isHovered = (mouseY < tabHeight && 
                              mouseX >= i * tabWidth && mouseX < (i + 1) * tabWidth);
            
            SettingsColorRGBA tabColor = isActive ? m_config.activeTabColor : 
                                 isHovered ? m_config.inactiveTabColor : m_config.tabBarColor;
            SDL_SetRenderDrawColor(m_renderer, tabColor.r, tabColor.g, tabColor.b, 255);
            SDL_RenderFillRect(m_renderer, &tabRect);
            
            if (isActive) {
                SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                        m_config.accentColor.g,
                                        m_config.accentColor.b, 255);
                SDL_FRect indicator = {(float)(i * tabWidth), (float)(tabHeight - 3), (float)tabWidth, 3.0f};
                SDL_RenderFillRect(m_renderer, &indicator);
            }
            
            renderText(tabNames[i], i * tabWidth + tabWidth / 2, 18, 16,
                       m_config.textColor, true);
        }
    }

    void SettingsWindow::renderPerformanceTab() {
        int y = 70;
        int contentX = 50;
        
        if (renderSection("Frame Rate", y)) {
            int fps = get("performance.target_fps").intVal;
            if (renderSliderInt("Target FPS", fps, 15, 144, 
                                 "Lower for better battery life")) {
                set("performance.target_fps", SettingValue{SettingValue::Int, 
                                                          false, fps});
                if (m_onChange) m_onChange("performance.target_fps");
            }
            
            int btnY = y + 10;
            int btnW = 150, btnH = 30, spacing = 10;
            struct Preset { const char* label; int fps; };
            Preset presets[] = {
                {"Power Saver (15)", 15},
                {"Balanced (30)", 30},
                {"Smooth (60)", 60},
                {"Max (144)", 144}
            };
            
            for (int i = 0; i < 4; ++i) {
                SDL_Rect btn = {contentX + i * (btnW + spacing), btnY, btnW, btnH};
                if (renderButton(presets[i].label, btn)) {
                    set("performance.target_fps", SettingValue{
                        SettingValue::Int, false, presets[i].fps});
                    if (m_onChange) m_onChange("performance.target_fps");
                }
            }
            y = btnY + btnH + 30;
        }
        
        if (renderSection("Entity Count", y)) {
            int count = get("performance.entity_count").intVal;
            if (renderSliderInt("Creature Count", count, 1000, 500000, 
                                 "More creatures = more emergent behavior")) {
                set("performance.entity_count", SettingValue{
                    SettingValue::Int, false, count});
                if (m_onChange) m_onChange("performance.entity_count");
            }
            y += 60;
        }
        
        if (renderSection("Smart Pausing", y)) {
            bool pauseFullscreen = get("performance.auto_pause_fullscreen").boolVal;
            if (renderToggle("Pause when fullscreen app is in front", 
                              pauseFullscreen,
                              "Saves CPU/GPU when playing games or watching videos")) {
                set("performance.auto_pause_fullscreen", SettingValue{
                    SettingValue::Bool, pauseFullscreen});
                if (m_onChange) m_onChange("performance.auto_pause_fullscreen");
            }
            
            bool respectBattery = get("performance.respect_battery").boolVal;
            if (renderToggle("Reduce quality on battery", respectBattery,
                              "Lower FPS and effects to save battery")) {
                set("performance.respect_battery", SettingValue{
                    SettingValue::Bool, respectBattery});
                if (m_onChange) m_onChange("performance.respect_battery");
            }
            y += 80;
        }
    }

    void SettingsWindow::renderVisualTab() {
        int y = 70;
        
        if (renderSection("Visual Effects", y)) {
            float glow = get("visual.glow_intensity").floatVal;
            if (renderSliderFloat("Glow Intensity", glow, 0.0f, 1.0f,
                                    "Bloom effect on luminous creatures")) {
                set("visual.glow_intensity", SettingValue{
                    SettingValue::Float, false, 0, glow});
                if (m_onChange) m_onChange("visual.glow_intensity");
            }
            
            float particles = get("visual.particle_density").floatVal;
            if (renderSliderFloat("Particle Density", particles, 0.0f, 1.0f)) {
                set("visual.particle_density", SettingValue{
                    SettingValue::Float, false, 0, particles});
                if (m_onChange) m_onChange("visual.particle_density");
            }
            
            float motion = get("visual.motion_speed").floatVal;
            if (renderSliderFloat("Motion Speed", motion, 0.1f, 3.0f)) {
                set("visual.motion_speed", SettingValue{
                    SettingValue::Float, false, 0, motion});
                if (m_onChange) m_onChange("visual.motion_speed");
            }
            
            float vignette = get("visual.vignette_intensity").floatVal;
            if (renderSliderFloat("Vignette", vignette, 0.0f, 1.0f)) {
                set("visual.vignette_intensity", SettingValue{
                    SettingValue::Float, false, 0, vignette});
                if (m_onChange) m_onChange("visual.vignette_intensity");
            }
            y += 200;
        }
        
        if (renderSection("Debug Overlays", y)) {
            bool showGrid = get("visual.show_grid_overlay").boolVal;
            if (renderToggle("Show spatial hash grid (F3)", showGrid,
                              "Visualize the spatial partitioning structure")) {
                set("visual.show_grid_overlay", SettingValue{SettingValue::Bool, showGrid});
                if (m_onChange) m_onChange("visual.show_grid_overlay");
            }
            y += 50;
        }
    }

    void SettingsWindow::renderAudioTab() {
        int y = 70;
        
        if (renderSection("Audio", y)) {
            bool audioEnabled = get("audio.enabled").boolVal;
            if (renderToggle("Enable audio", audioEnabled)) {
                set("audio.enabled", SettingValue{SettingValue::Bool, audioEnabled});
                if (m_onChange) m_onChange("audio.enabled");
            }
            
            float volume = get("audio.volume").floatVal;
            if (renderSliderFloat("Master Volume", volume, 0.0f, 1.0f)) {
                set("audio.volume", SettingValue{
                    SettingValue::Float, false, 0, volume});
                if (m_onChange) m_onChange("audio.volume");
            }
            
            bool muteOnFocus = get("audio.mute_on_focus_loss").boolVal;
            if (renderToggle("Mute when window loses focus", muteOnFocus)) {
                set("audio.mute_on_focus_loss", SettingValue{
                    SettingValue::Bool, muteOnFocus});
                if (m_onChange) m_onChange("audio.mute_on_focus_loss");
            }
            y += 150;
        }
    }

    void SettingsWindow::renderGameplayTab() {
        int y = 70;
        
        if (renderSection("Simulation", y)) {
            float speed = get("gameplay.simulation_speed").floatVal;
            if (renderSliderFloat("Base Speed", speed, 0.25f, 4.0f)) {
                set("gameplay.simulation_speed", SettingValue{
                    SettingValue::Float, false, 0, speed});
                if (m_onChange) m_onChange("gameplay.simulation_speed");
            }
            
            bool autoPause = get("gameplay.auto_pause_on_idle").boolVal;
            if (renderToggle("Auto-pause when idle", autoPause)) {
                set("gameplay.auto_pause_on_idle", SettingValue{
                    SettingValue::Bool, autoPause});
                if (m_onChange) m_onChange("gameplay.auto_pause_on_idle");
            }
            
            int idleTime = get("gameplay.idle_threshold").intVal;
            if (renderSliderInt("Idle threshold (seconds)", idleTime, 60, 1800)) {
                set("gameplay.idle_threshold", SettingValue{
                    SettingValue::Int, false, idleTime});
                if (m_onChange) m_onChange("gameplay.idle_threshold");
            }
            
            int diffIdx = 0;
            std::string diff = get("gameplay.difficulty").stringVal;
            if (diff == "Easy") diffIdx = 0;
            else if (diff == "Normal") diffIdx = 1;
            else if (diff == "Hard") diffIdx = 2;
            std::vector<std::string> options = {"Easy", "Normal", "Hard"};
            if (renderDropdown("Difficulty", diffIdx, options)) {
                set("gameplay.difficulty", SettingValue{
                    SettingValue::String, false, 0, 0.0f, options[diffIdx]});
                if (m_onChange) m_onChange("gameplay.difficulty");
            }
            y += 200;
        }
    }

    void SettingsWindow::renderControlsTab() {
        int y = 70;
        
        if (renderSection("Camera Controls", y)) {
            float panSpeed = get("controls.pan_speed").floatVal;
            if (renderSliderFloat("Pan Speed", panSpeed, 0.1f, 3.0f)) {
                set("controls.pan_speed", SettingValue{
                    SettingValue::Float, false, 0, panSpeed});
                if (m_onChange) m_onChange("controls.pan_speed");
            }
            
            float zoomSpeed = get("controls.zoom_speed").floatVal;
            if (renderSliderFloat("Zoom Speed", zoomSpeed, 0.1f, 3.0f)) {
                set("controls.zoom_speed", SettingValue{
                    SettingValue::Float, false, 0, zoomSpeed});
                if (m_onChange) m_onChange("controls.zoom_speed");
            }
            
            bool invertZoom = get("controls.invert_zoom").boolVal;
            if (renderToggle("Invert zoom direction", invertZoom)) {
                set("controls.invert_zoom", SettingValue{
                    SettingValue::Bool, invertZoom});
                if (m_onChange) m_onChange("controls.invert_zoom");
            }
            y += 150;
        }
        
        if (renderSection("Key Bindings", y)) {
            renderText("Pan Camera: WASD or Arrow Keys", 50, y + 10, 14, 
                       m_config.textColor);
            renderText("Zoom: Mouse Wheel", 50, y + 35, 14, m_config.textColor);
            renderText("Select Entity: Left Click", 50, y + 60, 14, m_config.textColor);
            renderText("Pause/Resume: Space", 50, y + 85, 14, m_config.textColor);
            renderText("Speed Controls: [ ]", 50, y + 110, 14, m_config.textColor);
            renderText("Save: F5  |  Load: F9", 50, y + 135, 14, m_config.textColor);
            renderText("Menu: ESC  |  Inspector: Tab", 50, y + 160, 14, m_config.textColor);
            y += 200;
        }
    }

    void SettingsWindow::renderModsTab() {
        int y = 70;
        
        if (renderSection("Installed Mods", y)) {
            renderText("No mods installed yet.", 50, y + 10, 14, 
                       m_config.secondaryTextColor);
            renderText("Mods extend the game with new behaviors, themes, and content.",
                       50, y + 35, 12, m_config.secondaryTextColor);
            
            SDL_Rect browseBtn = {50, y + 80, 200, 40};
            renderButton("Browse Steam Workshop", browseBtn);
            
            SDL_Rect docsBtn = {270, y + 80, 200, 40};
            renderButton("Modding Guide", docsBtn);
        }
    }

    void SettingsWindow::renderAccountTab() {
        int y = 70;
        
        if (renderSection("Steam Account", y)) {
            renderText("Status: Not signed in to Steam", 50, y + 10, 14,
                       m_config.warningColor);
            
            SDL_Rect loginBtn = {50, y + 50, 150, 35};
            renderButton("Sign in to Steam", loginBtn);
            y += 100;
        }
        
        if (renderSection("Profile", y)) {
            renderText("Player Name: Anonymous", 50, y + 10, 14,
                       m_config.textColor);
            
            SDL_Rect changeBtn = {50, y + 50, 150, 35};
            renderButton("Change Name", changeBtn);
        }
    }

    bool SettingsWindow::renderSection(const std::string& title, int& y) {
        renderText(title, 50, y, 18, m_config.accentColor);
        y += 30;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);

        SDL_SetRenderDrawColor(m_renderer, 60, 70, 85, 200);
        SDL_RenderLine(m_renderer, 50.0f, (float)(y - 5), 
                        (float)(winW - 50), (float)(y - 5));
        return true;
    }

    bool SettingsWindow::renderToggle(const std::string& label, bool& value, 
                                       const std::string& description) {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);

        int y = m_tempY;
        int x = 50;
        int toggleWidth = 50, toggleHeight = 24;
        
        renderText(label, x, y + 3, 15, m_config.textColor);
        
        if (!description.empty()) {
            renderText(description, x, y + 25, 12, m_config.secondaryTextColor);
        }
        
        int toggleX = winW - 100;
        SDL_FRect toggleRect = {(float)toggleX, (float)y, (float)toggleWidth, (float)toggleHeight};
        
        SettingsColorRGBA toggleBg = value ? 
            SettingsColorRGBA{80, 160, 220, 255} :
            SettingsColorRGBA{60, 65, 75, 255};
        SDL_SetRenderDrawColor(m_renderer, toggleBg.r, toggleBg.g, toggleBg.b, 255);
        SDL_RenderFillRect(m_renderer, &toggleRect);
        
        SDL_FRect knob = {
            value ? (float)(toggleX + toggleWidth - toggleHeight - 2) : (float)(toggleX + 2),
            (float)(y + 2), (float)(toggleHeight - 4), (float)(toggleHeight - 4)
        };
        SDL_SetRenderDrawColor(m_renderer, 240, 240, 245, 255);
        SDL_RenderFillRect(m_renderer, &knob);
        
        m_tempY = y + (description.empty() ? 35 : 55);
        return false;
    }

    bool SettingsWindow::renderSliderInt(const std::string& label, int& value, 
                                          int min, int max, const std::string& description) {
        (void)description;
        renderText(label + ": " + std::to_string(value), 50, m_tempY, 15, 
                   m_config.textColor);
        
        int sliderX = 50, sliderY = m_tempY + 25, sliderW = 400, sliderH = 8;
        SDL_FRect track = {(float)sliderX, (float)sliderY, (float)sliderW, (float)sliderH};
        SDL_SetRenderDrawColor(m_renderer, 40, 45, 55, 255);
        SDL_RenderFillRect(m_renderer, &track);
        
        float t = (float)(value - min) / (float)(max - min);
        SDL_FRect fill = {(float)sliderX, (float)sliderY, (float)(sliderW * t), (float)sliderH};
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, m_config.accentColor.b, 255);
        SDL_RenderFillRect(m_renderer, &fill);
        
        SDL_FRect knob = {(float)(sliderX + sliderW * t - 8), (float)(sliderY - 4), 16.0f, 16.0f};
        SDL_SetRenderDrawColor(m_renderer, 240, 240, 245, 255);
        SDL_RenderFillRect(m_renderer, &knob);
        
        m_tempY += 50;
        return false;
    }

    bool SettingsWindow::renderSliderFloat(const std::string& label, float& value, 
                                            float min, float max, const std::string& description) {
        (void)description;
        char valBuf[32];
        snprintf(valBuf, sizeof(valBuf), "%.2f", value);
        renderText(label + ": " + valBuf, 50, m_tempY, 15, m_config.textColor);
        
        int sliderX = 50, sliderY = m_tempY + 25, sliderW = 400, sliderH = 8;
        SDL_FRect track = {(float)sliderX, (float)sliderY, (float)sliderW, (float)sliderH};
        SDL_SetRenderDrawColor(m_renderer, 40, 45, 55, 255);
        SDL_RenderFillRect(m_renderer, &track);
        
        float t = (value - min) / (max - min);
        SDL_FRect fill = {(float)sliderX, (float)sliderY, (float)(sliderW * t), (float)sliderH};
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, m_config.accentColor.b, 255);
        SDL_RenderFillRect(m_renderer, &fill);
        
        SDL_FRect knob = {(float)(sliderX + sliderW * t - 8), (float)(sliderY - 4), 16.0f, 16.0f};
        SDL_SetRenderDrawColor(m_renderer, 240, 240, 245, 255);
        SDL_RenderFillRect(m_renderer, &knob);
        
        m_tempY += 50;
        return false;
    }

    bool SettingsWindow::renderDropdown(const std::string& label, int& selected, 
                                        const std::vector<std::string>& options, 
                                        const std::string& description) {
        (void)description;
        std::string sel = (selected >= 0 && selected < (int)options.size()) ? options[selected] : "None";
        renderText(label + ": [" + sel + "]", 50, m_tempY, 15, m_config.textColor);
        m_tempY += 35;
        return false;
    }

    bool SettingsWindow::renderButton(const std::string& label, SDL_Rect rect) {
        if (!m_renderer) return false;
        SDL_FRect fRect = {(float)rect.x, (float)rect.y, (float)rect.w, (float)rect.h};
        SDL_SetRenderDrawColor(m_renderer, 40, 50, 65, 255);
        SDL_RenderFillRect(m_renderer, &fRect);
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r, m_config.accentColor.g, m_config.accentColor.b, 255);
        SDL_RenderRect(m_renderer, &fRect);
        renderText(label, rect.x + rect.w / 2, rect.y + rect.h / 2 - 6, 14, m_config.textColor, true);
        return false;
    }

    void SettingsWindow::renderText(const std::string& text, int x, int y, int size, 
                                     SettingsColorRGBA color, bool centered) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        float drawX = centered ? (float)x - (text.length() * 4.0f) : (float)x;
        SDL_RenderDebugText(m_renderer, drawX, (float)y, text.c_str());
    }

    SettingsWindow::SettingValue SettingsWindow::get(const std::string& key) const {
        auto it = m_settings.find(key);
        if (it != m_settings.end()) return it->second;
        return SettingValue{};
    }

    void SettingsWindow::set(const std::string& key, const SettingValue& value) {
        m_settings[key] = value;
    }

    bool SettingsWindow::has(const std::string& key) const {
        return m_settings.find(key) != m_settings.end();
    }

    void SettingsWindow::reset() {
        m_settings.clear();
    }

    bool SettingsWindow::save(const std::string& path) const {
        try {
            std::ofstream f(path);
            if (!f.is_open()) return false;
            f << "{\n";
            size_t count = 0;
            for (const auto& [key, value] : m_settings) {
                f << "  \"" << key << "\": ";
                switch (value.type) {
                    case SettingValue::Bool:   f << (value.boolVal ? "true" : "false"); break;
                    case SettingValue::Int:    f << value.intVal; break;
                    case SettingValue::Float:  f << value.floatVal; break;
                    case SettingValue::String: f << "\"" << value.stringVal << "\""; break;
                }
                if (++count < m_settings.size()) f << ",";
                f << "\n";
            }
            f << "}\n";
            return true;
        } catch (...) {
            return false;
        }
    }

    bool SettingsWindow::load(const std::string& path) {
        try {
            std::ifstream f(path);
            if (!f.is_open()) return false;
            std::string line;
            while (std::getline(f, line)) {
                auto colPos = line.find(':');
                if (colPos == std::string::npos) continue;
                auto keyStart = line.find('"');
                auto keyEnd = line.find('"', keyStart + 1);
                if (keyStart == std::string::npos || keyEnd == std::string::npos || keyEnd >= colPos) continue;
                std::string key = line.substr(keyStart + 1, keyEnd - keyStart - 1);
                std::string valStr = line.substr(colPos + 1);

                // Trim whitespace and trailing comma
                while (!valStr.empty() && (valStr.back() == ' ' || valStr.back() == '\t' || valStr.back() == '\r' || valStr.back() == '\n' || valStr.back() == ',')) {
                    valStr.pop_back();
                }
                while (!valStr.empty() && (valStr.front() == ' ' || valStr.front() == '\t')) {
                    valStr.erase(valStr.begin());
                }

                SettingValue sv;
                if (valStr == "true" || valStr == "false") {
                    sv = SettingValue{SettingValue::Bool, valStr == "true"};
                } else if (!valStr.empty() && valStr.front() == '"' && valStr.back() == '"') {
                    sv = SettingValue{SettingValue::String, false, 0, 0.0f, valStr.substr(1, valStr.length() - 2)};
                } else if (valStr.find('.') != std::string::npos) {
                    sv = SettingValue{SettingValue::Float, false, 0, std::stof(valStr)};
                } else if (!valStr.empty()) {
                    sv = SettingValue{SettingValue::Int, false, std::stoi(valStr)};
                }
                set(key, sv);
            }
            return true;
        } catch (...) {
            return false;
        }
    }

} // namespace ShapeEngine::UI
