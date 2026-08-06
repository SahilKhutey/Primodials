// Engine/UI/Menus/MainMenu.h
#pragma once

#include <string>
#include <vector>
#include <functional>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace ShapeEngine::UI {

    struct MenuColorRGBA {
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;

        MenuColorRGBA() = default;
        MenuColorRGBA(float r_, float g_, float b_, float a_ = 1.0f) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// Main title screen menu with animated background and large buttons.
    /// Shows on game launch and from ESC.
    /// </summary>
    class MainMenu {
    public:
        enum class MenuAction {
            None,
            NewGame,
            Continue,
            LoadGame,
            OpenWorkshop,
            OpenSettings,
            OpenCredits,
            Quit
        };

        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            std::string backgroundVideo = "assets/ui/menu_bg.webm";
            std::string backgroundImage = "assets/ui/menu_bg.png";
            bool useVideoBackground = true;
            
            std::string logoImage = "assets/ui/logo_large.png";
            float logoY = 0.15f;
            
            std::string versionText = "v2.0.0";
            MenuColorRGBA versionColor = MenuColorRGBA(0.6f, 0.6f, 0.6f, 1.0f);
            float versionY = 0.95f;
            
            std::vector<std::string> buttonLabels = {
                "New Simulation",
                "Continue",
                "Load Game",
                "Workshop",
                "Settings",
                "Credits",
                "Quit"
            };
            std::vector<MenuAction> buttonActions = {
                MenuAction::NewGame,
                MenuAction::Continue,
                MenuAction::LoadGame,
                MenuAction::OpenWorkshop,
                MenuAction::OpenSettings,
                MenuAction::OpenCredits,
                MenuAction::Quit
            };
            
            float buttonWidth = 320.0f;
            float buttonHeight = 60.0f;
            float buttonSpacing = 15.0f;
            float buttonStartY = 0.40f;
            
            MenuColorRGBA buttonColor = MenuColorRGBA(0.15f, 0.15f, 0.20f, 0.85f);
            MenuColorRGBA buttonHoverColor = MenuColorRGBA(0.25f, 0.40f, 0.60f, 0.95f);
            MenuColorRGBA buttonTextColor = MenuColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);
            MenuColorRGBA buttonBorderColor = MenuColorRGBA(0.4f, 0.7f, 1.0f, 0.8f);
            float buttonFontSize = 22.0f;
            float buttonBorderThickness = 2.0f;
            float buttonBorderRadius = 8.0f;
            
            float backgroundScrollSpeed = 0.02f;
            float logoBobSpeed = 1.5f;
            float logoBobAmplitude = 8.0f;
            
            std::string menuMusicPath = "assets/audio/menu_theme.ogg";
            float musicVolume = 0.5f;
            
            bool showVersion = true;
        };

        explicit MainMenu(const Config& config);
        ~MainMenu();

        void render();
        void update(float deltaTime);

        MenuAction handleInput();
        MenuAction getHoveredAction() const { return m_hoveredAction; }
        void setHoveredAction(MenuAction action) { m_hoveredAction = action; }

        bool isVisible() const { return m_visible; }
        void setVisible(bool visible) { m_visible = visible; }
        void toggle() { m_visible = !m_visible; }

        using ActionCallback = std::function<void(MenuAction)>;
        void setActionCallback(ActionCallback cb) { m_actionCb = std::move(cb); }

        void playMenuMusic();
        void stopMenuMusic();

    private:
        void renderBackground();
        void renderLogo();
        void renderButtons();
        void renderVersion();
        void renderAnimatedEffects(float deltaTime);

        bool pointInButton(int index, int mouseX, int mouseY) const;
        void renderButton(int index, bool hovered);
        void renderText(const std::string& text, float x, float y, 
                         float size, const MenuColorRGBA& color);

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        
        bool m_visible = true;
        MenuAction m_hoveredAction = MenuAction::None;
        MenuAction m_lastClickedAction = MenuAction::None;
        
        float m_logoBobPhase = 0.0f;
        float m_backgroundScroll = 0.0f;
        float m_timeAccumulator = 0.0f;
        
        SDL_Texture* m_backgroundTexture = nullptr;
        SDL_Texture* m_logoTexture = nullptr;
        
        ActionCallback m_actionCb;
    };

} // namespace ShapeEngine::UI
