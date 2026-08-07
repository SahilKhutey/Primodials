// Engine/UI/Menus/PauseMenu.cpp
#include "PauseMenu.h"
#include "Core/Logger.hpp"

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    PauseMenu::PauseMenu(SDL_Window* window, SDL_Renderer* renderer)
        : m_window(window), m_renderer(renderer) {
    }

    PauseMenu::~PauseMenu() {
    }

    void PauseMenu::render() {
        if (!m_visible || !m_renderer) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);

        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, m_dimColor.r, m_dimColor.g, 
                                m_dimColor.b, m_dimColor.a);
        SDL_FRect fullScreen = {0.0f, 0.0f, (float)winW, (float)winH};
        SDL_RenderFillRect(m_renderer, &fullScreen);
        
        renderText("PAUSED", winW / 2 - 40, 150, 56, m_textColor);
        renderButtons();
    }

    void PauseMenu::renderButtons() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int buttonW = 300;
        int buttonH = 50;
        int spacing = 10;
        int startY = 280;
        int x = (winW - buttonW) / 2;
        
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);
        m_hoveredButton = -1;
        
        for (int i = 0; i < 6; ++i) {
            int y = startY + i * (buttonH + spacing);
            
            bool hovered = (mouseX >= x && mouseX <= x + buttonW &&
                            mouseY >= y && mouseY <= y + buttonH);
            
            if (hovered) m_hoveredButton = i;
            
            SDL_Color color = hovered ? m_buttonHoverColor : m_buttonColor;
            SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
            SDL_FRect rect = {(float)x, (float)y, (float)buttonW, (float)buttonH};
            SDL_RenderFillRect(m_renderer, &rect);
            
            if (hovered) {
                SDL_SetRenderDrawColor(m_renderer, 100, 150, 220, 255);
                SDL_RenderRect(m_renderer, &rect);
            }
            
            renderText(m_buttonLabels[i], x + buttonW / 2 - 40, y + 18, 
                       20, m_textColor);
        }
    }

    PauseMenu::PauseAction PauseMenu::handleInput() {
        PauseAction result = m_selectedAction;
        m_selectedAction = PauseAction::None;
        
        if (!m_visible) return result;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && 
                event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = (int)event.button.x;
                int mouseY = (int)event.button.y;
                
                int winW = 1280, winH = 720;
                GetWindowDims(m_window, winW, winH);

                for (int i = 0; i < 6; ++i) {
                    int buttonW = 300, buttonH = 50;
                    int x = (winW - buttonW) / 2;
                    int y = 280 + i * (buttonH + 10);
                    
                    if (mouseX >= x && mouseX <= x + buttonW &&
                        mouseY >= y && mouseY <= y + buttonH) {
                        result = m_buttonActions[i];
                        break;
                    }
                }
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    hide();
                    result = PauseAction::Resume;
                }
            }
        }
        
        return result;
    }

    void PauseMenu::renderText(const std::string& text, int x, int y, int size, 
                                SDL_Color color) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDebugText(m_renderer, (float)x, (float)y, text.c_str());
    }

} // namespace ShapeEngine::UI
