// Engine/UI/Heatmap/HeatmapToggle.cpp
#include "HeatmapToggle.h"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>
#include <vector>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    HeatmapToggle::HeatmapToggle(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
    }

    HeatmapToggle::~HeatmapToggle() = default;

    std::string HeatmapToggle::getModeName(HeatmapMode mode) const {
        switch (mode) {
            case HeatmapMode::None: return "Off";
            case HeatmapMode::PopulationDensity: return "Population Density";
            case HeatmapMode::EnergyDistribution: return "Energy Distribution";
            case HeatmapMode::GeneMutationRate: return "Mutation Hotspots";
            case HeatmapMode::PheromoneTrails: return "Pheromones";
            case HeatmapMode::Temperature: return "Temperature";
        }
        return "Unknown";
    }

    void HeatmapToggle::update(float deltaTime) {
        (void)deltaTime;
    }

    bool HeatmapToggle::handleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_H) {
                    int next = ((int)m_currentMode + 1) % 6;
                    m_currentMode = (HeatmapMode)next;
                    m_showLegend = (m_currentMode != HeatmapMode::None);
                    return true;
                }
            }
        }
        return false;
    }

    void HeatmapToggle::render() {
        if (!m_renderer) return;
        renderModeSelector();
        if (m_showLegend && m_currentMode != HeatmapMode::None) {
            renderLegend();
        }
    }

    void HeatmapToggle::renderModeSelector() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int x = 20;
        int y = winH - 60;
        int w = 220;
        int h = 35;
        
        SDL_FRect panel = {(float)x, (float)y, (float)w, (float)h};
        SDL_SetRenderDrawColor(m_renderer, m_config.backgroundColor.r,
                                m_config.backgroundColor.g, 
                                m_config.backgroundColor.b, m_config.backgroundColor.a);
        SDL_RenderFillRect(m_renderer, &panel);
        SDL_SetRenderDrawColor(m_renderer, m_config.borderColor.r,
                                m_config.borderColor.g, 
                                m_config.borderColor.b, 255);
        SDL_RenderRect(m_renderer, &panel);
        
        std::string modeText = "Heatmap (H): " + getModeName(m_currentMode);
        renderText(modeText, x + 15, y + 10, 12, m_config.textColor);
    }

    void HeatmapToggle::renderLegend() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int x = 20;
        int y = winH - 180;
        int w = 220;
        int h = 110;
        
        SDL_FRect panel = {(float)x, (float)y, (float)w, (float)h};
        SDL_SetRenderDrawColor(m_renderer, m_config.backgroundColor.r,
                                m_config.backgroundColor.g, 
                                m_config.backgroundColor.b, m_config.backgroundColor.a);
        SDL_RenderFillRect(m_renderer, &panel);
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, 
                                m_config.accentColor.b, 200);
        SDL_RenderRect(m_renderer, &panel);
        
        renderText(getModeName(m_currentMode) + " Legend", x + 10, y + 8, 12, m_config.accentColor);
        
        int rampX = x + 15;
        int rampY = y + 35;
        int rampW = w - 30;
        int rampH = 15;
        
        for (int i = 0; i < rampW; ++i) {
            float t = (float)i / (float)rampW;
            Uint8 r = (Uint8)(t * 255);
            Uint8 g = (Uint8)((1.0f - fabsf(t - 0.5f) * 2.0f) * 255);
            Uint8 b = (Uint8)((1.0f - t) * 255);
            
            SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
            SDL_RenderLine(m_renderer, (float)(rampX + i), (float)rampY, (float)(rampX + i), (float)(rampY + rampH));
        }
        
        SDL_FRect rampBorder = {(float)rampX, (float)rampY, (float)rampW, (float)rampH};
        SDL_SetRenderDrawColor(m_renderer, 200, 200, 210, 255);
        SDL_RenderRect(m_renderer, &rampBorder);
        
        renderText("Low", rampX, rampY + 20, 10, {180, 180, 190, 255});
        renderText("Medium", rampX + rampW / 2, rampY + 20, 10, {180, 180, 190, 255}, true);
        renderText("High", rampX + rampW, rampY + 20, 10, {180, 180, 190, 255}, true);
    }

    void HeatmapToggle::renderText(const std::string& text, int x, int y, int size, 
                                     HeatmapColorRGBA color, bool centered) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        float drawX = centered ? (float)x - (text.length() * 4.0f) : (float)x;
        SDL_RenderDebugText(m_renderer, drawX, (float)y, text.c_str());
    }

} // namespace ShapeEngine::UI
