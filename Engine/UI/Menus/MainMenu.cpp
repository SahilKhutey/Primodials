// Engine/UI/Menus/MainMenu.cpp
#include "MainMenu.h"
#include "Core/Logger.hpp"

#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    MainMenu::MainMenu(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
        playMenuMusic();
    }

    MainMenu::~MainMenu() {
        stopMenuMusic();
        if (m_backgroundTexture) SDL_DestroyTexture(m_backgroundTexture);
        if (m_logoTexture) SDL_DestroyTexture(m_logoTexture);
    }

    void MainMenu::update(float deltaTime) {
        if (!m_visible) return;
        
        m_timeAccumulator += deltaTime;
        m_logoBobPhase += deltaTime * m_config.logoBobSpeed;
        m_backgroundScroll += deltaTime * m_config.backgroundScrollSpeed;
    }

    void MainMenu::render() {
        if (!m_visible || !m_window || !m_renderer) return;
        
        SDL_SetRenderTarget(m_renderer, nullptr);
        renderBackground();
        renderLogo();
        renderButtons();
        renderVersion();
    }

    void MainMenu::renderBackground() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);

        if (m_backgroundTexture) {
            float texW = 0.0f, texH = 0.0f;
            SDL_GetTextureSize(m_backgroundTexture, &texW, &texH);
            if (texW <= 0.0f || texH <= 0.0f) { texW = 1920.0f; texH = 1080.0f; }
            
            float scaleX = (float)winW / texW;
            float scaleY = (float)winH / texH;
            float scale = std::max(scaleX, scaleY);
            
            SDL_FRect dst = {
                std::fmod(-m_backgroundScroll * texW, texW * scale) - texW * scale,
                0.0f,
                texW * scale * 3.0f,
                (float)winH
            };
            SDL_RenderTexture(m_renderer, m_backgroundTexture, nullptr, &dst);
        } else {
            SDL_SetRenderDrawColor(m_renderer, 8, 8, 15, 255);
            SDL_RenderClear(m_renderer);
        }
        
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 128);
        SDL_FRect overlay = {0.0f, 0.0f, (float)winW, (float)winH};
        SDL_RenderFillRect(m_renderer, &overlay);
    }

    void MainMenu::renderLogo() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);

        if (m_logoTexture) {
            float texW = 0.0f, texH = 0.0f;
            SDL_GetTextureSize(m_logoTexture, &texW, &texH);
            float bobOffset = std::sin(m_logoBobPhase) * m_config.logoBobAmplitude;
            float y = (winH * m_config.logoY) - (texH / 2.0f) + bobOffset;
            float x = (winW - texW) / 2.0f;
            SDL_FRect dst = {x, y, texW, texH};
            SDL_RenderTexture(m_renderer, m_logoTexture, nullptr, &dst);
        } else {
            renderText("PRIMORDIALS", winW / 2.0f - 80.0f, winH * 0.15f, 24.0f, MenuColorRGBA(0.4f, 0.9f, 1.0f, 1.0f));
        }
    }

    void MainMenu::renderButtons() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (size_t i = 0; i < m_config.buttonLabels.size(); ++i) {
            bool hovered = (m_hoveredAction == m_config.buttonActions[i]);
            
            if (pointInButton((int)i, (int)mouseX, (int)mouseY)) {
                hovered = true;
                m_hoveredAction = m_config.buttonActions[i];
            }
            
            renderButton((int)i, hovered);
        }
    }

    void MainMenu::renderButton(int index, bool hovered) {
        if (index >= (int)m_config.buttonLabels.size()) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);

        float totalHeight = m_config.buttonLabels.size() * 
                           (m_config.buttonHeight + m_config.buttonSpacing);
        float startY = (winH - totalHeight) / 2.0f;
        
        float x = (winW - m_config.buttonWidth) / 2.0f;
        float y = startY + index * (m_config.buttonHeight + m_config.buttonSpacing);
        
        MenuColorRGBA color = hovered ? m_config.buttonHoverColor : m_config.buttonColor;
        SDL_SetRenderDrawColor(m_renderer,
            (Uint8)(color.r * 255), (Uint8)(color.g * 255),
            (Uint8)(color.b * 255), (Uint8)(color.a * 255));
        SDL_FRect rect = {x, y, m_config.buttonWidth, m_config.buttonHeight};
        
        SDL_RenderFillRect(m_renderer, &rect);
        
        if (hovered) {
            SDL_SetRenderDrawColor(m_renderer,
                (Uint8)(m_config.buttonBorderColor.r * 255),
                (Uint8)(m_config.buttonBorderColor.g * 255),
                (Uint8)(m_config.buttonBorderColor.b * 255), 255);
            for (int i = 0; i < (int)m_config.buttonBorderThickness; ++i) {
                SDL_FRect borderRect = {x - i, y - i, 
                                        m_config.buttonWidth + i * 2, 
                                        m_config.buttonHeight + i * 2};
                SDL_RenderRect(m_renderer, &borderRect);
            }
        }
        
        renderText(m_config.buttonLabels[index],
                   x + m_config.buttonWidth / 2.0f - 60.0f,
                   y + m_config.buttonHeight / 2.0f - 6.0f,
                   m_config.buttonFontSize,
                   m_config.buttonTextColor);
    }

    void MainMenu::renderVersion() {
        if (!m_config.showVersion) return;
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);

        renderText(m_config.versionText, (float)(winW - 100), 
                   (float)(winH * m_config.versionY), 14.0f, m_config.versionColor);
    }

    MainMenu::MenuAction MainMenu::handleInput() {
        MenuAction action = m_lastClickedAction;
        m_lastClickedAction = MenuAction::None;
        
        if (!m_visible) return action;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && 
                event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = (int)event.button.x;
                int mouseY = (int)event.button.y;
                
                for (size_t i = 0; i < m_config.buttonActions.size(); ++i) {
                    if (pointInButton((int)i, mouseX, mouseY)) {
                        action = m_config.buttonActions[i];
                        if (m_actionCb) m_actionCb(action);
                        break;
                    }
                }
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_UP) {
                    int currentIdx = -1;
                    for (size_t i = 0; i < m_config.buttonActions.size(); ++i) {
                        if (m_config.buttonActions[i] == m_hoveredAction) {
                            currentIdx = (int)i;
                            break;
                        }
                    }
                    if (currentIdx > 0) m_hoveredAction = m_config.buttonActions[currentIdx - 1];
                    else m_hoveredAction = m_config.buttonActions.back();
                }
                else if (event.key.key == SDLK_DOWN) {
                    int currentIdx = -1;
                    for (size_t i = 0; i < m_config.buttonActions.size(); ++i) {
                        if (m_config.buttonActions[i] == m_hoveredAction) {
                            currentIdx = (int)i;
                            break;
                        }
                    }
                    if (currentIdx >= 0 && currentIdx < (int)m_config.buttonActions.size() - 1) {
                        m_hoveredAction = m_config.buttonActions[currentIdx + 1];
                    } else {
                        m_hoveredAction = m_config.buttonActions[0];
                    }
                }
                else if (event.key.key == SDLK_RETURN || event.key.key == SDLK_SPACE) {
                    if (m_hoveredAction != MenuAction::None) {
                        action = m_hoveredAction;
                        if (m_actionCb) m_actionCb(action);
                    }
                }
                else if (event.key.key == SDLK_ESCAPE) {
                    action = MenuAction::Quit;
                    if (m_actionCb) m_actionCb(action);
                }
            }
        }
        
        return action;
    }

    bool MainMenu::pointInButton(int index, int mouseX, int mouseY) const {
        if (index >= (int)m_config.buttonLabels.size()) return false;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);

        float totalHeight = m_config.buttonLabels.size() * 
                           (m_config.buttonHeight + m_config.buttonSpacing);
        float startY = (winH - totalHeight) / 2.0f;
        
        float x = (winW - m_config.buttonWidth) / 2.0f;
        float y = startY + index * (m_config.buttonHeight + m_config.buttonSpacing);
        
        return mouseX >= x && mouseX <= x + m_config.buttonWidth &&
               mouseY >= y && mouseY <= y + m_config.buttonHeight;
    }

    void MainMenu::renderText(const std::string& text, float x, float y, 
                                float size, const MenuColorRGBA& color) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer,
            (Uint8)(color.r * 255), (Uint8)(color.g * 255),
            (Uint8)(color.b * 255), (Uint8)(color.a * 255));
        SDL_RenderDebugText(m_renderer, x, y, text.c_str());
    }

    void MainMenu::playMenuMusic() {
    }

    void MainMenu::stopMenuMusic() {
    }

    void MainMenu::renderAnimatedEffects(float deltaTime) {
        (void)deltaTime;
    }

} // namespace ShapeEngine::UI
