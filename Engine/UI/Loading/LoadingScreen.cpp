// Engine/UI/Loading/LoadingScreen.cpp
#include "LoadingScreen.h"
#include "Core/Logger.hpp"

#include <SDL3/SDL.h>
#include <algorithm>
#include <random>
#include <cmath>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    LoadingScreen::LoadingScreen(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
        
        if (!m_config.tips.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, (int)m_config.tips.size() - 1);
            m_currentTipIndex = dis(gen);
            m_currentTip = m_config.tips[m_currentTipIndex];
        }
        
        m_startTime = std::chrono::steady_clock::now();
        m_fadingIn = true;
    }

    LoadingScreen::~LoadingScreen() {
        if (m_titleTexture) SDL_DestroyTexture(m_titleTexture);
        if (m_logoTexture) SDL_DestroyTexture(m_logoTexture);
        if (m_progressBarTexture) SDL_DestroyTexture(m_progressBarTexture);
        if (m_progressBarFillTexture) SDL_DestroyTexture(m_progressBarFillTexture);
    }

    void LoadingScreen::setState(LoadingState state, const std::string& statusText) {
        m_currentState = state;
        if (!statusText.empty()) m_statusText = statusText;
        else {
            switch (state) {
                case LoadingState::Initializing: m_statusText = "Initializing..."; break;
                case LoadingState::LoadingAssets: m_statusText = "Loading assets..."; break;
                case LoadingState::LoadingThemes: m_statusText = "Loading themes..."; break;
                case LoadingState::GeneratingWorld: m_statusText = "Generating world..."; break;
                case LoadingState::ConnectingMultiplayer: m_statusText = "Connecting..."; break;
                case LoadingState::Saving: m_statusText = "Saving..."; break;
                case LoadingState::Loading: m_statusText = "Loading..."; break;
                case LoadingState::GeneratingPreview: m_statusText = "Generating preview..."; break;
                case LoadingState::Complete: m_statusText = "Complete!"; break;
            }
        }
        
        if (state != m_currentState) {
            m_progress = 0.0f;
        }
    }

    void LoadingScreen::setProgress(float progress) {
        m_progress = std::clamp(progress, 0.0f, 1.0f);
    }

    void LoadingScreen::setStatusText(const std::string& text) {
        m_statusText = text;
    }

    void LoadingScreen::setTip(const std::string& tip) {
        m_currentTip = tip;
    }

    void LoadingScreen::setError(const std::string& error) {
        m_errorMessage = error;
        m_hasError = true;
        SHAPE_LOG_ERROR("Loading error: %s", error.c_str());
    }

    void LoadingScreen::startLoading(const std::string& initialStatus) {
        m_isLoading = true;
        m_hasError = false;
        m_errorMessage.clear();
        m_progress = 0.0f;
        if (!initialStatus.empty()) m_statusText = initialStatus;
        startFadeIn();
    }

    void LoadingScreen::finishLoading() {
        m_isLoading = false;
        m_progress = 1.0f;
        startFadeOut();
    }

    void LoadingScreen::update(float deltaTime) {
        if (m_fadingIn && m_fadeAlpha < 1.0f) {
            m_fadeAlpha += deltaTime / m_config.fadeInDuration;
            if (m_fadeAlpha >= 1.0f) {
                m_fadeAlpha = 1.0f;
                m_fadingIn = false;
            }
        }
        if (m_fadingOut && m_fadeAlpha > 0.0f) {
            m_fadeAlpha -= deltaTime / m_config.fadeOutDuration;
            if (m_fadeAlpha <= 0.0f) {
                m_fadeAlpha = 0.0f;
                m_fadingOut = false;
            }
        }
        
        m_rotationAngle += m_config.rotationSpeed * deltaTime;
        if (m_rotationAngle >= 360.0f) m_rotationAngle -= 360.0f;
        
        if (m_config.showTips && !m_config.tips.empty()) {
            m_timeSinceLastTip += deltaTime;
            if (m_timeSinceLastTip >= 5.0f) {
                cycleTip();
                m_timeSinceLastTip = 0.0f;
            }
        }
    }

    void LoadingScreen::render() {
        if (!m_window || !m_renderer) return;
        if (m_fadeAlpha <= 0.0f && !m_fadingIn) return;
        
        SDL_SetRenderTarget(m_renderer, nullptr);
        
        SDL_SetRenderDrawColor(m_renderer,
            (Uint8)(m_config.backgroundColor.r * 255),
            (Uint8)(m_config.backgroundColor.g * 255),
            (Uint8)(m_config.backgroundColor.b * 255),
            (Uint8)(m_config.backgroundColor.a * 255 * m_fadeAlpha));
        SDL_RenderClear(m_renderer);
        
        renderAnimatedElements(1.0f / 60.0f);
        
        if (m_config.showLogo && m_logoTexture) {
            renderLogo();
        }
        
        if (m_titleTexture) {
            int winW = 1280, winH = 720;
            GetWindowDims(m_window, winW, winH);
            float w = 0.0f, h = 0.0f;
            SDL_GetTextureSize(m_titleTexture, &w, &h);
            SDL_FRect dst = {
                (winW - w) / 2.0f,
                100.0f,
                w,
                h
            };
            SDL_RenderTexture(m_renderer, m_titleTexture, nullptr, &dst);
        }
        
        if (m_config.showProgressBar) {
            renderProgressBar();
        }
        
        renderStatusText();
        
        if (m_config.showTips) {
            renderTip();
        }
        
        if (m_hasError) {
            renderError();
        }
        
        SDL_RenderPresent(m_renderer);
    }

    void LoadingScreen::renderBackground() {
        if (m_config.animateBackground) {
            int winW = 1280, winH = 720;
            GetWindowDims(m_window, winW, winH);
            float t = (float)SDL_GetTicks() / 1000.0f;
            for (int i = 0; i < 10; ++i) {
                float offset = std::sin(t + i * 0.5f) * 0.1f;
                SDL_SetRenderDrawColor(m_renderer, 
                    (Uint8)(i * 25), (Uint8)(i * 25), (Uint8)(i * 30), 
                    (Uint8)(30 * m_fadeAlpha));
                SDL_FRect bar = {
                    0, (float)(i * 60 + offset * 50),
                    (float)winW,
                    40.0f
                };
                SDL_RenderFillRect(m_renderer, &bar);
            }
        }
    }

    void LoadingScreen::renderAnimatedElements(float deltaTime) {
        (void)deltaTime;
        renderBackground();
    }

    void LoadingScreen::renderLogo() {
        if (!m_logoTexture) return;
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        float w = 0.0f, h = 0.0f;
        SDL_GetTextureSize(m_logoTexture, &w, &h);
        SDL_FRect dst = {
            (winW - w) / 2.0f,
            40.0f,
            w,
            h
        };
        SDL_RenderTexture(m_renderer, m_logoTexture, nullptr, &dst);
    }

    void LoadingScreen::renderProgressBar() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        float barWidth = 400.0f;
        float barHeight = 20.0f;
        float barX = (winW - barWidth) / 2.0f;
        float barY = winH * 0.75f;
        
        SDL_SetRenderDrawColor(m_renderer, 40, 40, 50, 
            (Uint8)(200 * m_fadeAlpha));
        SDL_FRect bg = {barX, barY, barWidth, barHeight};
        SDL_RenderFillRect(m_renderer, &bg);
        
        SDL_SetRenderDrawColor(m_renderer, 
            (Uint8)(m_config.progressBarColor.r * 255),
            (Uint8)(m_config.progressBarColor.g * 255),
            (Uint8)(m_config.progressBarColor.b * 255),
            (Uint8)(255 * m_fadeAlpha));
        SDL_FRect fill = {barX, barY, barWidth * m_progress, barHeight};
        SDL_RenderFillRect(m_renderer, &fill);
        
        SDL_SetRenderDrawColor(m_renderer, 100, 100, 120, 
            (Uint8)(255 * m_fadeAlpha));
        SDL_RenderRect(m_renderer, &bg);
        
        char pctText[16];
        snprintf(pctText, sizeof(pctText), "%.0f%%", m_progress * 100.0f);
        renderText(pctText, barX + barWidth / 2 - 20, barY, 16, 
                   m_config.textColor);
    }

    void LoadingScreen::renderStatusText() {
        if (m_statusText.empty()) return;
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        renderText(m_statusText, winW / 2.0f - 100, 
                   winH * 0.7f, 20, 
                   m_config.textColor);
    }

    void LoadingScreen::renderTip() {
        if (m_currentTip.empty()) return;
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        renderText("TIP:", winW / 2.0f - 100, winH * 0.85f, 14, 
                   m_config.accentColor);
        
        renderText(m_currentTip, winW / 2.0f - 200, winH * 0.88f, 16, 
                   m_config.textColor);
    }

    void LoadingScreen::renderError() {
        if (!m_hasError || m_errorMessage.empty()) return;
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_SetRenderDrawColor(m_renderer, 80, 20, 20, 
            (Uint8)(200 * m_fadeAlpha));
        SDL_FRect box = {winW / 2.0f - 300, winH / 2.0f, 600, 100};
        SDL_RenderFillRect(m_renderer, &box);
        
        renderText("ERROR: " + m_errorMessage, winW / 2.0f - 280, 
                   winH / 2.0f + 40, 18, ColorRGBA(1, 0.5f, 0.5f, m_fadeAlpha));
    }

    void LoadingScreen::renderText(const std::string& text, float x, float y, 
                                    float size, const ColorRGBA& color) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, 
            (Uint8)(color.r * 255), 
            (Uint8)(color.g * 255), 
            (Uint8)(color.b * 255), 
            (Uint8)(color.a * 255 * m_fadeAlpha));
        SDL_RenderDebugText(m_renderer, x, y, text.c_str());
    }

    void LoadingScreen::cycleTip() {
        if (m_config.tips.empty()) return;
        m_currentTipIndex = (m_currentTipIndex + 1) % m_config.tips.size();
        m_currentTip = m_config.tips[m_currentTipIndex];
    }

    void LoadingScreen::startFadeIn() {
        m_fadingIn = true;
        m_fadingOut = false;
        m_fadeAlpha = 0.0f;
    }

    void LoadingScreen::startFadeOut() {
        m_fadingOut = true;
        m_fadingIn = false;
    }

    SDL_Texture* LoadingScreen::loadTexture(const std::string& path) {
        (void)path;
        return nullptr;
    }

    void LoadingScreen::loadAssetsAsync(const std::vector<std::string>& assetPaths) {
        runAsync([this, assetPaths]() {
            for (size_t i = 0; i < assetPaths.size(); ++i) {
                setProgress((float)i / assetPaths.size());
            }
            setProgress(1.0f);
        }, "Loading assets...");
    }

} // namespace ShapeEngine::UI
