// Engine/UI/Onboarding/TutorialOverlay.cpp
#include "TutorialOverlay.h"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstdio>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    TutorialOverlay::TutorialOverlay(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
    }

    TutorialOverlay::~TutorialOverlay() = default;

    void TutorialOverlay::start() {
        m_active = true;
        m_complete = false;
        m_skipped = false;
        m_currentStep = 0;
        m_stepTimer = 0;
        m_cardOffset = 100.0f;
        SHAPE_LOG_INFO("Tutorial started");
    }

    void TutorialOverlay::stop() {
        m_active = false;
        m_complete = true;
    }

    void TutorialOverlay::skip() {
        m_skipped = true;
        m_active = false;
        SHAPE_LOG_INFO("Tutorial skipped");
        if (m_onSkipped) m_onSkipped();
    }

    void TutorialOverlay::nextStep() {
        m_currentStep++;
        m_stepTimer = 0;
        if (m_currentStep >= (int)m_config.steps.size()) {
            stop();
            if (m_onComplete) m_onComplete();
        }
    }

    void TutorialOverlay::previousStep() {
        if (m_currentStep > 0) {
            m_currentStep--;
            m_stepTimer = 0;
        }
    }

    void TutorialOverlay::update(float deltaTime) {
        if (!m_active) return;
        
        m_stepTimer += deltaTime;
        
        m_cardOffset += (m_cardTarget - m_cardOffset) * 5.0f * deltaTime;
        
        if (m_currentStep < (int)m_config.steps.size()) {
            const auto& step = m_config.steps[m_currentStep];
            
            if (step.duration > 0 && m_stepTimer >= step.duration) {
                nextStep();
            }
            else if (step.autoComplete && step.autoComplete()) {
                nextStep();
            }
        }
    }

    bool TutorialOverlay::handleInput() {
        if (!m_active) return false;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_SPACE || event.key.key == SDLK_RETURN) {
                    nextStep();
                    return true;
                }
                if (event.key.key == SDLK_BACKSPACE) {
                    previousStep();
                    return true;
                }
                if (event.key.key == SDLK_ESCAPE) {
                    skip();
                    return true;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                int winW = 1280, winH = 720;
                GetWindowDims(m_window, winW, winH);
                
                int cardX = (winW - m_config.cardWidth) / 2;
                int cardY = (winH - m_config.cardHeight) / 2 + (int)m_cardOffset;
                
                int btnW = 120, btnH = 36;
                int nextBtnX = cardX + m_config.cardWidth - btnW - 20;
                int nextBtnY = cardY + m_config.cardHeight - btnH - 20;
                
                if (event.button.x >= nextBtnX && event.button.x <= nextBtnX + btnW &&
                    event.button.y >= nextBtnY && event.button.y <= nextBtnY + btnH) {
                    nextStep();
                    return true;
                }
                
                int skipBtnX = cardX + 20;
                int skipBtnY = nextBtnY;
                if (event.button.x >= skipBtnX && event.button.x <= skipBtnX + btnW &&
                    event.button.y >= skipBtnY && event.button.y <= skipBtnY + btnH) {
                    skip();
                    return true;
                }
            }
        }
        return true;
    }

    void TutorialOverlay::render() {
        if (!m_active || !m_renderer) return;
        renderHighlight();
        renderCard();
        renderNavigation();
    }

    void TutorialOverlay::renderHighlight() {
        if (m_currentStep >= (int)m_config.steps.size()) return;
        const auto& step = m_config.steps[m_currentStep];
        
        if (step.highlightW > 0 && step.highlightH > 0) {
            SDL_FRect highlight = {
                step.highlightX, step.highlightY,
                step.highlightW, step.highlightH
            };
            
            int winW = 1280, winH = 720;
            GetWindowDims(m_window, winW, winH);
            
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 180);
            SDL_FRect fullScreen = {0.0f, 0.0f, (float)winW, (float)winH};
            SDL_RenderFillRect(m_renderer, &fullScreen);
            
            float pulse = (sinf((float)SDL_GetTicks() / 300.0f) + 1.0f) * 0.5f;
            SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, (Uint8)(100 + pulse * 100));
            for (int i = 0; i < 3; ++i) {
                SDL_FRect border = {
                    highlight.x - i, highlight.y - i,
                    highlight.w + i * 2, highlight.h + i * 2
                };
                SDL_RenderRect(m_renderer, &border);
            }
        }
    }

    void TutorialOverlay::renderCard() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int cardX = (winW - m_config.cardWidth) / 2;
        int cardY = (winH - m_config.cardHeight) / 2 + (int)m_cardOffset;
        
        if (m_currentStep >= (int)m_config.steps.size()) return;
        const auto& step = m_config.steps[m_currentStep];
        
        SDL_FRect shadow = {(float)(cardX + 5), (float)(cardY + 5), 
                            (float)m_config.cardWidth, (float)m_config.cardHeight};
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 100);
        SDL_RenderFillRect(m_renderer, &shadow);
        
        SDL_FRect card = {(float)cardX, (float)cardY, (float)m_config.cardWidth, (float)m_config.cardHeight};
        SDL_SetRenderDrawColor(m_renderer, m_config.cardColor.r,
                                m_config.cardColor.g, m_config.cardColor.b, 250);
        SDL_RenderFillRect(m_renderer, &card);
        
        SDL_SetRenderDrawColor(m_renderer, m_config.borderColor.r,
                                m_config.borderColor.g, m_config.borderColor.b, 255);
        SDL_RenderRect(m_renderer, &card);
        
        int dotY = cardY + 15;
        int dotSpacing = 12;
        int totalDots = (int)m_config.steps.size();
        int dotsWidth = (totalDots - 1) * dotSpacing;
        int dotsStartX = cardX + (m_config.cardWidth - dotsWidth) / 2;
        
        for (int i = 0; i < totalDots; ++i) {
            SDL_FRect dot = {(float)(dotsStartX + i * dotSpacing - 3), (float)dotY, 6.0f, 6.0f};
            TutorialColorRGBA dotColor = (i == m_currentStep) ? m_config.accentColor : TutorialColorRGBA{80, 90, 110, 255};
            SDL_SetRenderDrawColor(m_renderer, dotColor.r, dotColor.g, dotColor.b, 255);
            SDL_RenderFillRect(m_renderer, &dot);
        }
        
        renderText(step.title, cardX + 25, cardY + 35, 20, m_config.textColor);
        
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, m_config.accentColor.b, 200);
        SDL_RenderLine(m_renderer, (float)(cardX + 25), (float)(cardY + 65), 
                        (float)(cardX + m_config.cardWidth - 25), (float)(cardY + 65));
        
        renderWrappedText(step.body, cardX + 25, cardY + 75, 
                          m_config.cardWidth - 50, 14, m_config.textColor);
    }

    void TutorialOverlay::renderNavigation() {
        if (m_currentStep >= (int)m_config.steps.size()) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int cardX = (winW - m_config.cardWidth) / 2;
        int cardY = (winH - m_config.cardHeight) / 2 + (int)m_cardOffset;
        int btnW = 120, btnH = 36;
        int btnY = cardY + m_config.cardHeight - btnH - 20;
        
        button(cardX + 20, btnY, btnW, btnH, "Skip Tutorial", m_config.buttonColor);
        
        if (m_currentStep > 0) {
            int backBtnX = cardX + (m_config.cardWidth - btnW) / 2;
            button(backBtnX, btnY, btnW, btnH, "<- Back", m_config.buttonColor);
        }
        
        std::string nextLabel = (m_currentStep == (int)m_config.steps.size() - 1) ? "Finish" : "Next ->";
        int nextBtnX = cardX + m_config.cardWidth - btnW - 20;
        button(nextBtnX, btnY, btnW, btnH, nextLabel, m_config.buttonHoverColor);
        
        char progStr[64];
        snprintf(progStr, sizeof(progStr), "Step %d of %d", 
                 m_currentStep + 1, (int)m_config.steps.size());
        renderText(progStr, cardX + m_config.cardWidth / 2, 
                   btnY - 18, 11, m_config.secondaryTextColor, true);
    }

    bool TutorialOverlay::button(int x, int y, int w, int h, 
                                    const std::string& label, TutorialColorRGBA color) {
        if (!m_renderer) return false;
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool hovered = (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
        
        TutorialColorRGBA btnColor = hovered ? m_config.buttonHoverColor : color;
        SDL_SetRenderDrawColor(m_renderer, btnColor.r, btnColor.g, btnColor.b, 255);
        SDL_FRect btn = {(float)x, (float)y, (float)w, (float)h};
        SDL_RenderFillRect(m_renderer, &btn);
        SDL_SetRenderDrawColor(m_renderer, 80, 90, 110, 255);
        SDL_RenderRect(m_renderer, &btn);
        
        renderText(label, x + w / 2, y + (h / 2 - 6), 13, m_config.textColor, true);
        return hovered;
    }

    void TutorialOverlay::renderText(const std::string& text, int x, int y, int size, 
                                        TutorialColorRGBA color, bool centered) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        float drawX = centered ? (float)x - (text.length() * 4.0f) : (float)x;
        SDL_RenderDebugText(m_renderer, drawX, (float)y, text.c_str());
    }

    void TutorialOverlay::renderWrappedText(const std::string& text, int x, int y, 
                                             int w, int size, TutorialColorRGBA color) {
        (void)w;
        std::istringstream iss(text);
        std::string line;
        int lineY = y;
        while (std::getline(iss, line, '\n')) {
            renderText(line, x, lineY, size, color);
            lineY += 18;
        }
    }

} // namespace ShapeEngine::UI
