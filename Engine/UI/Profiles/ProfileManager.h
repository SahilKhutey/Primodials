// Engine/UI/Profiles/ProfileManager.h
#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>

namespace ShapeEngine::UI {

    struct ProfileColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        ProfileColorRGBA() = default;
        ProfileColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// Save/load/delete named profiles containing all settings.
    /// Profiles bundle theme + visual settings + audio + gameplay settings.
    /// </summary>
    class ProfileManager {
    public:
        struct Profile {
            std::string name;
            std::string createdDate;
            std::string lastUsedDate;
            std::string themeId = "deep-sea";
            std::string settingsJson = "{}";
            SDL_Texture* preview = nullptr;
            bool isBuiltIn = false;
        };

        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            int panelWidth = 600;
            int panelHeight = 500;
            
            ProfileColorRGBA backgroundColor = {18, 20, 26, 250};
            ProfileColorRGBA cardColor = {30, 35, 45, 255};
            ProfileColorRGBA cardHoverColor = {50, 60, 80, 255};
            ProfileColorRGBA accentColor = {100, 180, 255, 255};
            ProfileColorRGBA textColor = {230, 235, 245, 255};
            ProfileColorRGBA secondaryTextColor = {140, 150, 170, 255};
            ProfileColorRGBA successColor = {130, 220, 150, 255};
            ProfileColorRGBA dangerColor = {255, 100, 100, 255};
        };

        explicit ProfileManager(const Config& config);
        ~ProfileManager();

        void render();
        bool handleInput();
        void update(float deltaTime);

        void show() { m_visible = true; }
        void hide() { m_visible = false; }
        void toggle() { m_visible = !m_visible; }
        bool isVisible() const { return m_visible; }

        void refreshProfiles();
        bool saveCurrentAsProfile(const std::string& name);
        bool loadProfile(const std::string& name);
        bool deleteProfile(const std::string& name);
        
        const std::vector<Profile>& getProfiles() const { return m_profiles; }
        std::string getActiveProfile() const { return m_activeProfile; }

        using ProfileLoadedCallback = std::function<void(const std::string& profile)>;
        void setOnProfileLoaded(ProfileLoadedCallback cb) { m_onLoaded = std::move(cb); }
        
        using ProfileSavedCallback = std::function<void(const std::string& profile)>;
        void setOnProfileSaved(ProfileSavedCallback cb) { m_onSaved = std::move(cb); }

        void showNameInputDialog();

    private:
        void renderProfileList();
        void renderProfileCard(Profile& profile, int index);
        void renderNewProfileDialog();
        void renderDeleteConfirmation();
        void renderText(const std::string& text, int x, int y, int size, 
                         ProfileColorRGBA color, bool centered = false);
        bool button(int x, int y, int w, int h, const std::string& label, 
                     ProfileColorRGBA color);
        SDL_Texture* generatePreview(const Profile& profile);
        std::string getCurrentDateTime();

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_visible = false;
        
        std::vector<Profile> m_profiles;
        std::string m_activeProfile = "Default";
        int m_hoveredIndex = -1;
        int m_scrollOffset = 0;
        
        bool m_showNameDialog = false;
        char m_newNameBuffer[64] = "";
        std::string m_dialogError;
        
        bool m_showDeleteConfirm = false;
        std::string m_profileToDelete;
        
        ProfileLoadedCallback m_onLoaded;
        ProfileSavedCallback m_onSaved;
    };

} // namespace ShapeEngine::UI
