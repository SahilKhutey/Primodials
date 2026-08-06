// Engine/UI/Menus/PauseMenu.h
#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace ShapeEngine::UI {

    class PauseMenu {
    public:
        enum class PauseAction {
            None,
            Resume,
            Save,
            Load,
            Settings,
            MainMenu,
            Quit
        };

        PauseMenu(SDL_Window* window, SDL_Renderer* renderer);
        ~PauseMenu();

        void render();
        PauseAction handleInput();
        bool isVisible() const { return m_visible; }
        void show() { m_visible = true; }
        void hide() { m_visible = false; }
        void toggle() { m_visible = !m_visible; }

    private:
        void renderBackground();
        void renderButtons();
        void renderText(const std::string& text, int x, int y, int size, 
                         SDL_Color color);

        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_visible = false;
        PauseAction m_selectedAction = PauseAction::None;
        int m_hoveredButton = -1;
        
        const char* m_buttonLabels[6] = {
            "Resume", "Save Game", "Load Game", "Settings", "Main Menu", "Quit"
        };
        PauseAction m_buttonActions[6] = {
            PauseAction::Resume, PauseAction::Save, PauseAction::Load,
            PauseAction::Settings, PauseAction::MainMenu, PauseAction::Quit
        };
        
        SDL_Color m_textColor = {255, 255, 255, 255};
        SDL_Color m_dimColor = {0, 0, 0, 180};
        SDL_Color m_buttonColor = {40, 50, 70, 220};
        SDL_Color m_buttonHoverColor = {70, 100, 150, 240};
    };

} // namespace ShapeEngine::UI
