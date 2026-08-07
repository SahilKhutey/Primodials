// Engine/UI/Performance/PerformanceOverlay.cpp
#include "PerformanceOverlay.h"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <vector>
#include <cstdio>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    PerformanceOverlay::PerformanceOverlay(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
    }

    PerformanceOverlay::~PerformanceOverlay() = default;

    void PerformanceOverlay::recordFrame(float deltaTime, int entityCount, 
                                          float simSpeed) {
        float frameMs = std::max(0.001f, deltaTime * 1000.0f);
        FrameData data;
        data.frameTime = frameMs;
        data.entityCount = entityCount;
        m_history.push_back(data);
        
        if ((int)m_history.size() > m_config.historySize) {
            m_history.pop_front();
        }
        
        float fps = 1000.0f / frameMs;
        m_avgFPS = m_avgFPS * 0.95f + fps * 0.05f;
        m_minFPS = std::min(m_minFPS, fps);
        m_maxFPS = std::max(m_maxFPS, fps);
        m_avgFrameTime = m_avgFrameTime * 0.95f + frameMs * 0.05f;
        m_currentEntities = entityCount;
        m_simSpeed = simSpeed;
    }

    void PerformanceOverlay::update(float deltaTime) {
        (void)deltaTime;
    }

    void PerformanceOverlay::render() {
        if (!m_visible || !m_renderer) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int x = winW - m_config.overlayWidth - m_config.margin;
        int y = m_config.margin + 40;
        
        SDL_FRect bg = {(float)x, (float)y, (float)m_config.overlayWidth, (float)m_config.overlayHeight};
        SDL_SetRenderDrawColor(m_renderer, m_config.backgroundColor.r,
                                m_config.backgroundColor.g, 
                                m_config.backgroundColor.b, m_config.backgroundColor.a);
        SDL_RenderFillRect(m_renderer, &bg);
        SDL_SetRenderDrawColor(m_renderer, 60, 70, 90, 150);
        SDL_RenderRect(m_renderer, &bg);
        
        renderText("Performance", x + 10, y + 5, 13, m_config.textColor);
        renderText("(F4 toggle)", x + m_config.overlayWidth - 75, y + 8, 9, 
                   {180, 180, 190, 200});
        
        renderStats();
        renderFPSGraph();
    }

    void PerformanceOverlay::renderStats() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int x = winW - m_config.overlayWidth - 20;
        int y = 60;
        
        int textY = y + 25;
        
        PerformanceColorRGBA fpsColor = (m_avgFPS >= 50) ? m_config.fpsGoodColor :
                                        (m_avgFPS >= 30) ? m_config.fpsOkayColor :
                                                            m_config.fpsBadColor;
        char fpsStr[64];
        snprintf(fpsStr, sizeof(fpsStr), "FPS: %.1f", m_avgFPS);
        renderText(fpsStr, x + 10, textY, 13, fpsColor);
        textY += 16;
        
        snprintf(fpsStr, sizeof(fpsStr), "  min: %.0f max: %.0f", m_minFPS, m_maxFPS);
        renderText(fpsStr, x + 10, textY, 10, {180, 180, 190, 255});
        textY += 14;
        
        snprintf(fpsStr, sizeof(fpsStr), "Frame: %.2f ms", m_avgFrameTime);
        renderText(fpsStr, x + 10, textY, 11, {200, 200, 210, 255});
        textY += 14;
        
        snprintf(fpsStr, sizeof(fpsStr), "Entities: %d", m_currentEntities);
        renderText(fpsStr, x + 10, textY, 11, {200, 200, 210, 255});
        textY += 14;
        
        snprintf(fpsStr, sizeof(fpsStr), "Sim Speed: %.2fx", m_simSpeed);
        renderText(fpsStr, x + 10, textY, 11, {200, 200, 210, 255});
    }

    void PerformanceOverlay::renderFPSGraph() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int x = winW - m_config.overlayWidth - 20;
        int y = 60;
        int graphX = x + 10;
        int graphY = y + 130;
        int graphW = m_config.overlayWidth - 20;
        int graphH = 50;
        
        SDL_FRect graphBg = {(float)graphX, (float)graphY, (float)graphW, (float)graphH};
        SDL_SetRenderDrawColor(m_renderer, 30, 35, 45, 255);
        SDL_RenderFillRect(m_renderer, &graphBg);
        
        SDL_SetRenderDrawColor(m_renderer, m_config.gridColor.r,
                                m_config.gridColor.g, 
                                m_config.gridColor.b, 150);
        for (int fps = 30; fps <= 60; fps += 15) {
            float lineY = (float)graphY + (float)graphH - ((float)fps / 60.0f * (float)graphH);
            SDL_RenderLine(m_renderer, (float)graphX, lineY, (float)(graphX + graphW), lineY);
        }
        
        if (m_history.size() > 1) {
            int numPoints = (int)m_history.size();
            float xStep = (float)graphW / (float)(m_config.historySize - 1);
            
            std::vector<SDL_FPoint> points;
            points.reserve(numPoints);
            for (int i = 0; i < numPoints; ++i) {
                const auto& frame = m_history[i];
                float fps = 1000.0f / frame.frameTime;
                fps = std::clamp(fps, 0.0f, 70.0f);
                float px = (float)graphX + i * xStep;
                float py = (float)graphY + (float)graphH - ((fps / 60.0f) * (float)graphH);
                points.push_back({px, py});
            }
            
            SDL_SetRenderDrawColor(m_renderer, m_config.lineColor.r,
                                    m_config.lineColor.g, 
                                    m_config.lineColor.b, 255);
            SDL_RenderLines(m_renderer, points.data(), (int)points.size());
        }
        
        renderText("60 FPS", graphX + graphW - 40, graphY - 10, 8, {180, 180, 190, 200});
        renderText("30 FPS", graphX + graphW - 40, graphY + graphH - 12, 8, {180, 180, 190, 200});
    }

    void PerformanceOverlay::renderText(const std::string& text, int x, int y, int size, 
                                          PerformanceColorRGBA color) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDebugText(m_renderer, (float)x, (float)y, text.c_str());
    }

} // namespace ShapeEngine::UI
