// Engine/UI/HUD/EntityInspector.cpp
#include "EntityInspector.h"
#include "Core/Logger.hpp"

#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    EntityInspector::EntityInspector(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
    }

    EntityInspector::~EntityInspector() = default;

    void EntityInspector::setSelectedEntity(EntityId entity) {
        m_selectedEntity = entity;
        m_scrollOffset = 0;
    }

    void EntityInspector::update(float deltaTime) {
        (void)deltaTime;
    }

    bool EntityInspector::handleInput() {
        if (!m_visible) return false;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            int mouseX = 0, mouseY = 0;
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                mouseX = (int)event.motion.x;
                mouseY = (int)event.motion.y;
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                mouseX = (int)event.button.x;
                mouseY = (int)event.button.y;
            }
            
            if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                m_scrollOffset -= (int)(event.wheel.y * 30.0f);
                m_scrollOffset = std::clamp(m_scrollOffset, 0, m_maxScroll);
            }
            
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                SDL_Rect closeBtn = {winW - m_config.panelWidth + 10, 10, 25, 25};
                if (mouseX >= closeBtn.x && mouseX <= closeBtn.x + closeBtn.w &&
                    mouseY >= closeBtn.y && mouseY <= closeBtn.y + closeBtn.h) {
                    hide();
                    if (m_closeCallback) m_closeCallback();
                    return true;
                }
            }
            
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE || event.key.key == SDLK_TAB) {
                    hide();
                    if (m_closeCallback) m_closeCallback();
                    return true;
                }
            }
        }
        return true;
    }

    void EntityInspector::render() {
        if (!m_visible || m_selectedEntity == INVALID_ENTITY || !m_renderer) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int panelX = winW - m_config.panelWidth;
        int panelY = 50;
        
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, m_config.backgroundColor.r,
                                m_config.backgroundColor.g,
                                m_config.backgroundColor.b, 240);
        SDL_FRect panelRect = {(float)panelX, (float)panelY, (float)m_config.panelWidth, 
                               (float)(winH - panelY - 50)};
        SDL_RenderFillRect(m_renderer, &panelRect);
        
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, m_config.accentColor.b, 150);
        SDL_RenderRect(m_renderer, &panelRect);
        
        int y = panelY + 10;
        renderHeader();
        y += 60;
        
        if (m_config.showGenomeRadar) {
            if (renderSection("Genome", y)) {
                renderGenomeRadar();
                y += 180;
            }
        }
        
        if (m_config.showTraitBars) {
            if (renderSection("Traits", y)) {
                renderTraitBars();
                y += 220;
            }
        }
        
        if (m_config.showLineageTree) {
            if (renderSection("Lineage", y)) {
                renderLineageInfo(y);
            }
        }
        
        if (m_config.showRelationships) {
            if (renderSection("Relationships", y)) {
                renderRelationships(y);
            }
        }
        
        if (m_config.showHistory) {
            if (renderSection("History", y)) {
                renderHistory(y);
            }
        }
        
        renderActionButtons();
        renderCloseButton();
    }

    void EntityInspector::renderHeader() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = winW - m_config.panelWidth;
        
        SDL_SetRenderDrawColor(m_renderer, m_config.headerColor.r,
                                m_config.headerColor.g,
                                m_config.headerColor.b, 255);
        SDL_FRect header = {(float)panelX, 50.0f, (float)m_config.panelWidth, 50.0f};
        SDL_RenderFillRect(m_renderer, &header);
        
        char buf[64];
        snprintf(buf, sizeof(buf), "Entity #%llu", (unsigned long long)m_selectedEntity);
        renderText(buf, panelX + 15, 62, 18, m_config.textColor);
        
        if (m_cached.speciesId > 0) {
            char speciesBuf[32];
            snprintf(speciesBuf, sizeof(speciesBuf), "Species %u", m_cached.speciesId);
            renderText(speciesBuf, panelX + 15, 82, 12, m_config.secondaryTextColor);
        }
    }

    bool EntityInspector::renderSection(const std::string& title, int& y) {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = winW - m_config.panelWidth;
        
        renderText(title, panelX + 15, y, 15, m_config.accentColor);
        y += 22;
        
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, m_config.accentColor.b, 80);
        SDL_RenderLine(m_renderer, (float)(panelX + 15), (float)y, 
                        (float)(panelX + m_config.panelWidth - 15), (float)y);
        y += 8;
        return true;
    }

    void EntityInspector::renderGenomeRadar() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = winW - m_config.panelWidth;
        int cx = panelX + m_config.panelWidth / 2;
        int cy = 100 + 30;
        int radius = 80;
        
        SDL_SetRenderDrawColor(m_renderer, 50, 55, 65, 255);
        for (int i = 1; i <= 4; ++i) {
            drawCircle(cx, cy, radius * i / 4, 30);
        }
        
        SDL_SetRenderDrawColor(m_renderer, 60, 65, 75, 255);
        for (int i = 0; i < 8; ++i) {
            float angle = (i / 8.0f) * 6.2831853f - 1.5707963f;
            int x1 = cx + (int)(std::cos(angle) * radius);
            int y1 = cy + (int)(std::sin(angle) * radius);
            SDL_RenderLine(m_renderer, (float)cx, (float)cy, (float)x1, (float)y1);
        }
        
        renderRadarChart(cx, cy, radius, m_cached.traits, m_config.traitColors);
        
        const char* traitNames[] = {
            "SPD", "SIZ", "VIS", "AGR", "DEF", "MET", "REP", "LIF"
        };
        for (int i = 0; i < 8; ++i) {
            float angle = (i / 8.0f) * 6.2831853f - 1.5707963f;
            int lx = cx + (int)(std::cos(angle) * (radius + 15));
            int ly = cy + (int)(std::sin(angle) * (radius + 15));
            renderText(traitNames[i], lx, ly, 10, {200, 200, 210, 255}, true);
        }
    }

    void EntityInspector::renderRadarChart(int cx, int cy, int radius, 
                                              const float traits[8], 
                                              const InspectorColorRGBA colors[8]) {
        std::vector<SDL_Vertex> verts;
        SDL_Vertex center = {{(float)cx, (float)cy}, 
                              {(float)colors[0].r / 255.0f, (float)colors[0].g / 255.0f, (float)colors[0].b / 255.0f, (float)colors[0].a / 255.0f}, 
                              {0, 0}};
        verts.push_back(center);
        
        for (int i = 0; i < 8; ++i) {
            float angle = (i / 8.0f) * 6.2831853f - 1.5707963f;
            float value = std::clamp(traits[i], 0.0f, 1.0f);
            float x = cx + std::cos(angle) * radius * value;
            float y = cy + std::sin(angle) * radius * value;
            verts.push_back({{x, y}, 
                             {(colors[i].r * 0.6f + 100) / 255.0f, 
                              (colors[i].g * 0.6f + 100) / 255.0f, 
                              (colors[i].b * 0.6f + 100) / 255.0f, 
                              150.0f / 255.0f}, 
                             {0, 0}});
        }
        verts.push_back(verts[1]);
        
        SDL_RenderGeometry(m_renderer, nullptr, verts.data(), (int)verts.size(), nullptr, 0);
        
        SDL_SetRenderDrawColor(m_renderer, colors[0].r, colors[0].g, colors[0].b, 255);
        for (size_t i = 1; i < verts.size() - 1; ++i) {
            SDL_RenderLine(m_renderer, 
                            verts[i].position.x, verts[i].position.y,
                            verts[i + 1].position.x, verts[i + 1].position.y);
        }
        
        for (size_t i = 1; i < verts.size() - 1; ++i) {
            SDL_SetRenderDrawColor(m_renderer, colors[i - 1].r, 
                                    colors[i - 1].g, colors[i - 1].b, 255);
            drawCircle((int)verts[i].position.x, (int)verts[i].position.y, 4, 12);
        }
    }

    void EntityInspector::renderTraitBars() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = winW - m_config.panelWidth;
        int y = 280;
        
        const char* traitNames[] = {
            "Speed", "Size", "Vision", "Aggression", 
            "Defense", "Metabolism", "Reproduction", "Lifespan"
        };
        
        int barX = panelX + 100;
        int barW = m_config.panelWidth - 115;
        int rowHeight = 22;
        
        for (int i = 0; i < 8; ++i) {
            float value = std::clamp(m_cached.traits[i], 0.0f, 1.0f);
            
            renderText(traitNames[i], panelX + 10, y, 12, m_config.textColor);
            
            SDL_FRect barBg = {(float)barX, (float)(y + 2), (float)barW, 10.0f};
            SDL_SetRenderDrawColor(m_renderer, 40, 45, 55, 255);
            SDL_RenderFillRect(m_renderer, &barBg);
            
            SDL_FRect barFill = {(float)barX, (float)(y + 2), (float)(barW * value), 10.0f};
            SDL_SetRenderDrawColor(m_renderer, m_config.traitColors[i].r,
                                    m_config.traitColors[i].g, 
                                    m_config.traitColors[i].b, 255);
            SDL_RenderFillRect(m_renderer, &barFill);
            
            char valStr[8];
            snprintf(valStr, sizeof(valStr), "%.2f", value);
            renderText(valStr, barX + barW + 5, y, 10, m_config.secondaryTextColor);
            
            y += rowHeight;
        }
    }

    void EntityInspector::renderLineageInfo(int& y) {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = winW - m_config.panelWidth;
        
        char buf[64];
        snprintf(buf, sizeof(buf), "Generation: %d", m_cached.generation);
        renderText(buf, panelX + 15, y, 12, m_config.textColor);
        
        if (m_cached.generation >= 100) {
            InspectorColorRGBA genColor = (m_cached.generation >= 1000) ? 
                                  m_config.successColor : m_config.accentColor;
            renderText("Longevity Bonus", panelX + 200, y, 11, genColor);
        }
        y += 18;
        
        if (!m_cached.parents.empty()) {
            renderText("Parents:", panelX + 15, y, 11, m_config.secondaryTextColor);
            y += 15;
            for (const auto& parent : m_cached.parents) {
                renderText("- " + parent, panelX + 25, y, 10, m_config.secondaryTextColor);
                y += 13;
            }
        }
        y += 5;
        
        char childBuf[32];
        snprintf(childBuf, sizeof(childBuf), "Offspring: %d", m_cached.childrenCount);
        renderText(childBuf, panelX + 15, y, 12, m_config.textColor);
        y += 20;
    }

    void EntityInspector::renderRelationships(int& y) {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = winW - m_config.panelWidth;
        
        char buf[64];
        snprintf(buf, sizeof(buf), "Age: %.1f sec", m_cached.timeAlive);
        renderText(buf, panelX + 15, y, 12, m_config.textColor);
        y += 18;
        
        snprintf(buf, sizeof(buf), "Energy: %.0f / %.0f", 
                 m_cached.energy, m_cached.energyMax);
        renderText(buf, panelX + 15, y, 12, m_config.textColor);
        
        int barX = panelX + 15;
        int barW = m_config.panelWidth - 30;
        SDL_FRect energyBarBg = {(float)barX, (float)(y + 18), (float)barW, 6.0f};
        SDL_SetRenderDrawColor(m_renderer, 40, 45, 55, 255);
        SDL_RenderFillRect(m_renderer, &energyBarBg);
        
        float energyPct = m_cached.energyMax > 0 ? 
                          m_cached.energy / m_cached.energyMax : 0;
        SDL_FRect energyBarFill = {(float)barX, (float)(y + 18), (float)(barW * energyPct), 6.0f};
        InspectorColorRGBA energyColor = (energyPct > 0.5f) ? m_config.successColor : 
                                (energyPct > 0.2f) ? m_config.warningColor : 
                                m_config.dangerColor;
        SDL_SetRenderDrawColor(m_renderer, energyColor.r, energyColor.g, 
                                energyColor.b, 255);
        SDL_RenderFillRect(m_renderer, &energyBarFill);
        y += 35;
        
        snprintf(buf, sizeof(buf), "Kills: %d", m_cached.kills);
        renderText(buf, panelX + 15, y, 12, m_config.textColor);
        y += 20;
    }

    void EntityInspector::renderHistory(int& y) {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = winW - m_config.panelWidth;
        
        renderText("Recent Events:", panelX + 15, y, 11, m_config.secondaryTextColor);
        y += 16;
        
        renderText("- Spawned", panelX + 25, y, 10, m_config.secondaryTextColor);
        y += 13;
        renderText("- Ate food (+10 energy)", panelX + 25, y, 10, m_config.successColor);
        y += 13;
        renderText("- Moved 12 units", panelX + 25, y, 10, m_config.secondaryTextColor);
        y += 13;
        renderText("- Reproduced", panelX + 25, y, 10, m_config.accentColor);
        y += 13;
        renderText("- Age increased", panelX + 25, y, 10, m_config.secondaryTextColor);
        y += 13;
        
        if (m_cached.kills > 0) {
            char killBuf[32];
            snprintf(killBuf, sizeof(killBuf), "- Hunted prey (%d)", m_cached.kills);
            renderText(killBuf, panelX + 25, y, 10, m_config.dangerColor);
            y += 13;
        }
    }

    void EntityInspector::renderActionButtons() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = winW - m_config.panelWidth;
        int y = winH - 100;
        
        int btnW = (m_config.panelWidth - 40) / 2;
        int btnH = 30;
        
        SDL_Rect trackBtn = {panelX + 15, y, btnW, btnH};
        SDL_Rect followBtn = {panelX + 25 + btnW, y, btnW, btnH};
        
        if (renderButton("Track", trackBtn)) {
            if (m_actionCallback) m_actionCallback("track", m_selectedEntity);
        }
        if (renderButton("Follow", followBtn)) {
            if (m_actionCallback) m_actionCallback("follow", m_selectedEntity);
        }
        y += btnH + 10;
        
        SDL_Rect deselectBtn = {panelX + 15, y, m_config.panelWidth - 30, btnH};
        if (renderButton("Deselect (ESC)", deselectBtn)) {
            clearSelection();
            hide();
        }
    }

    void EntityInspector::renderCloseButton() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = winW - m_config.panelWidth;
        
        SDL_FRect closeBtn = {(float)(panelX + m_config.panelWidth - 35), 60.0f, 25.0f, 25.0f};
        SDL_SetRenderDrawColor(m_renderer, 100, 60, 60, 200);
        SDL_RenderFillRect(m_renderer, &closeBtn);
        
        SDL_SetRenderDrawColor(m_renderer, 220, 220, 220, 255);
        SDL_RenderLine(m_renderer, closeBtn.x + 6, closeBtn.y + 6, 
                        closeBtn.x + 19, closeBtn.y + 19);
        SDL_RenderLine(m_renderer, closeBtn.x + 19, closeBtn.y + 6, 
                        closeBtn.x + 6, closeBtn.y + 19);
    }

    void EntityInspector::drawCircle(int cx, int cy, int radius, int segments) {
        if (!m_renderer) return;
        std::vector<SDL_FPoint> points;
        for (int i = 0; i <= segments; ++i) {
            float angle = (i / (float)segments) * 6.2831853f;
            points.push_back({(float)(cx + std::cos(angle) * radius), 
                              (float)(cy + std::sin(angle) * radius)});
        }
        SDL_RenderLines(m_renderer, points.data(), (int)points.size());
    }

    bool EntityInspector::renderButton(const std::string& label, SDL_Rect rect) {
        if (!m_renderer) return false;
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool hovered = (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                        mouseY >= rect.y && mouseY <= rect.y + rect.h);
        
        InspectorColorRGBA btnColor = hovered ? 
            InspectorColorRGBA{60, 100, 150, 255} : InspectorColorRGBA{45, 55, 70, 255};
        SDL_SetRenderDrawColor(m_renderer, btnColor.r, btnColor.g, btnColor.b, 255);
        SDL_FRect fRect = {(float)rect.x, (float)rect.y, (float)rect.w, (float)rect.h};
        SDL_RenderFillRect(m_renderer, &fRect);
        
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, m_config.accentColor.b, 150);
        SDL_RenderRect(m_renderer, &fRect);
        
        renderText(label, rect.x + rect.w / 2, rect.y + 7, 12, 
                   m_config.textColor, true);
        
        return false;
    }

    void EntityInspector::renderText(const std::string& text, int x, int y, int size, 
                                       InspectorColorRGBA color, bool centered) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        float drawX = centered ? (float)x - (text.length() * 4.0f) : (float)x;
        SDL_RenderDebugText(m_renderer, drawX, (float)y, text.c_str());
    }

} // namespace ShapeEngine::UI
