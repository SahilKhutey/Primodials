// Engine/UI/ThemeSystem/ThemeSelector.cpp
#include "ThemeSelector.h"
#include "Themes/ThemeManager.h"
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

    ThemeSelector::ThemeSelector(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
        refreshThemes();
    }

    ThemeSelector::~ThemeSelector() {
        for (auto& entry : m_themeEntries) {
            if (entry.thumbnail) SDL_DestroyTexture(entry.thumbnail);
            if (entry.previewTexture) SDL_DestroyTexture(entry.previewTexture);
        }
    }

    void ThemeSelector::refreshThemes() {
        for (auto& entry : m_themeEntries) {
            if (entry.thumbnail) SDL_DestroyTexture(entry.thumbnail);
        }
        m_themeEntries.clear();
        m_themes.clear();
        
        Themes::ThemeManager tm;
        tm.initialize(m_config.themesDirectory);
        m_themes = tm.discoverThemes();
        
        for (const auto& theme : m_themes) {
            ThemeEntry entry;
            entry.theme = theme;
            entry.thumbnail = generateThumbnail(theme);
            m_themeEntries.push_back(std::move(entry));
        }
        
        int rows = (m_themeEntries.empty()) ? 0 : (int)(m_themeEntries.size() + m_config.thumbnailsPerRow - 1) / m_config.thumbnailsPerRow;
        int gridHeight = rows * (m_config.thumbnailSize + m_config.thumbnailSpacing + 40);
        m_maxScroll = std::max(0, gridHeight - (m_config.panelHeight - 150));
        
        SHAPE_LOG_INFO("Loaded %zu themes for selector", m_themeEntries.size());
    }

    void ThemeSelector::setSelectedTheme(const std::string& themeId) {
        m_selectedThemeId = themeId;
        m_currentThemeId = themeId;
    }

    void ThemeSelector::update(float deltaTime) {
        if (!m_visible) return;
        
        if (m_showPreview) {
            m_previewTimer -= deltaTime;
            if (m_previewTimer <= 0.0f) {
                m_showPreview = false;
                if (m_themeCallback && !m_currentThemeId.empty()) {
                    m_themeCallback(m_currentThemeId);
                }
            }
        }
    }

    bool ThemeSelector::handleInput() {
        if (!m_visible) return false;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_Rect closeBtn = {winW - 60, 20, 40, 40};
        SDL_Rect workshopBtn = {(winW - 200) / 2, winH - 60, 200, 40};
        
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
            
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (mouseX >= closeBtn.x && mouseX <= closeBtn.x + closeBtn.w &&
                    mouseY >= closeBtn.y && mouseY <= closeBtn.y + closeBtn.h) {
                    hide();
                    if (m_closeCallback) m_closeCallback();
                    return true;
                }
                if (mouseX >= workshopBtn.x && mouseX <= workshopBtn.x + workshopBtn.w &&
                    mouseY >= workshopBtn.y && mouseY <= workshopBtn.y + workshopBtn.h) {
                    if (m_workshopCallback) m_workshopCallback();
                    return true;
                }
            }
            
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    hide();
                    if (m_closeCallback) m_closeCallback();
                    return true;
                }
                if (event.key.key == SDLK_RETURN && !m_selectedThemeId.empty()) {
                    if (m_themeCallback) m_themeCallback(m_selectedThemeId);
                    m_currentThemeId = m_selectedThemeId;
                    return true;
                }
            }
            
            if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                m_scrollOffset -= (int)(event.wheel.y * 30.0f);
                m_scrollOffset = std::clamp(m_scrollOffset, 0, m_maxScroll);
            }
            
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                for (size_t i = 0; i < m_themeEntries.size(); ++i) {
                    auto& entry = m_themeEntries[i];
                    if (mouseX >= entry.cardRect.x && 
                        mouseX <= entry.cardRect.x + entry.cardRect.w &&
                        mouseY >= entry.cardRect.y - m_scrollOffset && 
                        mouseY <= entry.cardRect.y - m_scrollOffset + entry.cardRect.h) {
                        setSelectedTheme(entry.theme.id);
                        if (m_config.allowLivePreview) {
                            m_showPreview = true;
                            m_previewingThemeId = entry.theme.id;
                            m_previewTimer = m_config.previewDuration;
                            if (m_themeCallback) m_themeCallback(entry.theme.id);
                        }
                        break;
                    }
                }
            }
        }
        
        return true;
    }

    void ThemeSelector::render() {
        if (!m_visible || !m_renderer) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, m_config.backgroundColor.r, 
                                m_config.backgroundColor.g,
                                m_config.backgroundColor.b, 200);
        SDL_FRect bg = {0.0f, 0.0f, (float)winW, (float)winH};
        SDL_RenderFillRect(m_renderer, &bg);
        
        int panelX = (winW - m_config.panelWidth) / 2;
        int panelY = (winH - m_config.panelHeight) / 2;
        SDL_FRect panelRect = {(float)panelX, (float)panelY, (float)m_config.panelWidth, (float)m_config.panelHeight};
        
        SDL_SetRenderDrawColor(m_renderer, m_config.panelColor.r, 
                                m_config.panelColor.g,
                                m_config.panelColor.b, m_config.panelColor.a);
        SDL_RenderFillRect(m_renderer, &panelRect);
        
        SDL_SetRenderDrawColor(m_renderer, m_config.borderColor.r,
                                m_config.borderColor.g,
                                m_config.borderColor.b, m_config.borderColor.a);
        SDL_RenderRect(m_renderer, &panelRect);
        
        renderHeader();
        renderThemeGrid();
        renderFooter();
        renderCloseButton();
        renderWorkshopButton();
        
        if (m_showPreview) renderPreviewOverlay();
    }

    void ThemeSelector::renderHeader() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = (winW - m_config.panelWidth) / 2;
        int pY = panelY_start();

        renderText(m_config.title, panelX + 40, pY + 30, 
                   m_config.titleFontSize, m_config.textColor);
        renderText(m_config.subtitle, panelX + 40, pY + 75, 
                   m_config.subtitleFontSize, m_config.secondaryTextColor);
        
        SDL_SetRenderDrawColor(m_renderer, m_config.borderColor.r,
                                m_config.borderColor.g, m_config.borderColor.b, 200);
        SDL_RenderLine(m_renderer, (float)(panelX + 40), (float)(pY + 105),
                        (float)(panelX + m_config.panelWidth - 40), (float)(pY + 105));
    }

    void ThemeSelector::renderThemeGrid() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = (winW - m_config.panelWidth) / 2;
        int pY = panelY_start();
        int gridStartY = pY + 130;
        int gridEndY = pY + m_config.panelHeight - 80;
        
        SDL_Rect clipRect = {
            panelX + 20, gridStartY,
            m_config.panelWidth - 40 - m_config.scrollbarWidth,
            gridEndY - gridStartY
        };
        SDL_SetRenderClipRect(m_renderer, &clipRect);
        
        int cardX = panelX + 40;
        int cardY = gridStartY + 10 - m_scrollOffset;
        
        for (size_t i = 0; i < m_themeEntries.size(); ++i) {
            auto& entry = m_themeEntries[i];
            
            int col = (int)(i % m_config.thumbnailsPerRow);
            int row = (int)(i / m_config.thumbnailsPerRow);
            int x = cardX + col * (m_config.thumbnailSize + m_config.thumbnailSpacing);
            int y = cardY + row * (m_config.thumbnailSize + m_config.thumbnailSpacing + 40);
            
            entry.cardRect = {x, y, m_config.thumbnailSize, m_config.thumbnailSize + 40};
            
            if (y + m_config.thumbnailSize >= gridStartY && y <= gridEndY) {
                renderThemeCard(entry);
            }
        }
        
        SDL_SetRenderClipRect(m_renderer, nullptr);
        
        if (m_maxScroll > 0) {
            int scrollbarX = panelX + m_config.panelWidth - m_config.scrollbarWidth - 10;
            int scrollbarY = gridStartY;
            int scrollbarHeight = gridEndY - gridStartY;
            int thumbHeight = std::max(20, 
                (int)(scrollbarHeight * (gridEndY - gridStartY) / 
                      (m_maxScroll + (gridEndY - gridStartY))));
            int thumbY = scrollbarY + 
                (int)((float)scrollbarHeight * m_scrollOffset / m_maxScroll);
            
            SDL_SetRenderDrawColor(m_renderer, 40, 45, 55, 200);
            SDL_FRect track = {(float)scrollbarX, (float)scrollbarY, 
                               (float)m_config.scrollbarWidth, (float)scrollbarHeight};
            SDL_RenderFillRect(m_renderer, &track);
            
            SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                    m_config.accentColor.g, m_config.accentColor.b, 255);
            SDL_FRect thumb = {(float)scrollbarX, (float)thumbY, 
                               (float)m_config.scrollbarWidth, (float)thumbHeight};
            SDL_RenderFillRect(m_renderer, &thumb);
        }
    }

    void ThemeSelector::renderThemeCard(ThemeEntry& entry) {
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);
        int adjustedY = entry.cardRect.y - m_scrollOffset;
        
        bool hovered = (mouseX >= entry.cardRect.x && 
                        mouseX <= entry.cardRect.x + entry.cardRect.w &&
                        mouseY >= adjustedY && 
                        mouseY <= adjustedY + entry.cardRect.h);
        bool selected = (entry.theme.id == m_selectedThemeId);
        entry.isHovered = hovered;
        entry.isSelected = selected;
        
        SDL_FRect cardRect = {
            (float)entry.cardRect.x, (float)adjustedY,
            (float)entry.cardRect.w, (float)(entry.cardRect.h - 40)
        };
        
        UIThemeColorRGBA bgColor = {30, 35, 45, 255};
        SDL_SetRenderDrawColor(m_renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(m_renderer, &cardRect);
        
        if (entry.thumbnail) {
            SDL_RenderTexture(m_renderer, entry.thumbnail, nullptr, &cardRect);
        }
        
        UIThemeColorRGBA borderColor = selected ? m_config.selectedBorderColor :
                                hovered ? m_config.hoverBorderColor : m_config.borderColor;
        SDL_SetRenderDrawColor(m_renderer, borderColor.r, borderColor.g, 
                                borderColor.b, 255);
        SDL_RenderRect(m_renderer, &cardRect);
        
        renderText(entry.theme.displayName, 
                   entry.cardRect.x, (int)(adjustedY + cardRect.h + 5), 14, 
                   selected ? m_config.accentColor : m_config.textColor);
        
        std::string author = "by " + entry.theme.author;
        renderText(author, entry.cardRect.x, (int)(adjustedY + cardRect.h + 22), 11, 
                   m_config.secondaryTextColor);
    }

    void ThemeSelector::renderFooter() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = (winW - m_config.panelWidth) / 2;
        int footerY = panelY_start() + m_config.panelHeight - 60;
        
        SDL_SetRenderDrawColor(m_renderer, m_config.borderColor.r,
                                m_config.borderColor.g, m_config.borderColor.b, 200);
        SDL_RenderLine(m_renderer, (float)(panelX + 40), (float)(footerY - 5),
                        (float)(panelX + m_config.panelWidth - 40), (float)(footerY - 5));
        
        if (!m_selectedThemeId.empty()) {
            std::string status = "Selected: " + m_selectedThemeId + " (Press ENTER to apply)";
            renderText(status, panelX + 40, footerY + 15, 13, m_config.secondaryTextColor);
        } else {
            renderText("Click a theme to preview • ENTER to apply • ESC to close",
                       panelX + 40, footerY + 15, 13, m_config.secondaryTextColor);
        }
    }

    void ThemeSelector::renderCloseButton() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = (winW - m_config.panelWidth) / 2;
        
        int x = panelX + m_config.panelWidth - 60;
        int y = panelY_start() + 15;
        SDL_FRect btn = {(float)x, (float)y, 40.0f, 40.0f};
        
        SDL_SetRenderDrawColor(m_renderer, 60, 50, 55, 200);
        SDL_RenderFillRect(m_renderer, &btn);
        
        SDL_SetRenderDrawColor(m_renderer, 220, 100, 100, 255);
        SDL_RenderLine(m_renderer, (float)(x + 12), (float)(y + 12), (float)(x + 28), (float)(y + 28));
        SDL_RenderLine(m_renderer, (float)(x + 28), (float)(y + 12), (float)(x + 12), (float)(y + 28));
    }

    void ThemeSelector::renderWorkshopButton() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        int x = (winW - 220) / 2;
        int y = winH - 80;
        SDL_FRect btn = {(float)x, (float)y, 220.0f, 40.0f};
        
        SDL_SetRenderDrawColor(m_renderer, m_config.panelColor.r,
                                m_config.panelColor.g, m_config.panelColor.b, 200);
        SDL_RenderFillRect(m_renderer, &btn);
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, m_config.accentColor.b, 200);
        SDL_RenderRect(m_renderer, &btn);
        
        renderText("Browse Steam Workshop ->", x + 110, y + 12, 14, 
                   m_config.textColor, true);
    }

    void ThemeSelector::renderPreviewOverlay() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_FRect indicator = {(float)(winW - 280), 80.0f, 260.0f, 50.0f};
        SDL_SetRenderDrawColor(m_renderer, 80, 100, 140, 220);
        SDL_RenderFillRect(m_renderer, &indicator);
        
        char buf[64];
        snprintf(buf, sizeof(buf), "Previewing: %s (%.0fs)", 
                 m_previewingThemeId.c_str(), m_previewTimer);
        renderText(buf, (int)indicator.x + 10, (int)indicator.y + 18, 14, 
                   {255, 255, 255, 255});
    }

    void ThemeSelector::renderText(const std::string& text, int x, int y, int size, 
                                      UIThemeColorRGBA color, bool centered) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        float drawX = centered ? (float)x - (text.length() * 4.0f) : (float)x;
        SDL_RenderDebugText(m_renderer, drawX, (float)y, text.c_str());
    }

    SDL_Texture* ThemeSelector::generateThumbnail(const Themes::Theme& theme) {
        if (!m_renderer) return nullptr;
        SDL_Surface* surface = SDL_CreateSurface(
            m_config.thumbnailSize, m_config.thumbnailSize, 
            SDL_PIXELFORMAT_RGBA8888);
        
        if (!surface) return nullptr;
        
        SDL_LockSurface(surface);
        uint32_t* pixels = (uint32_t*)surface->pixels;
        int pitch = surface->pitch / 4;
        
        const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
        for (int y = 0; y < m_config.thumbnailSize; ++y) {
            float t = (float)y / m_config.thumbnailSize;
            float r = theme.backgroundBottomColor.x * (1 - t) + theme.backgroundTopColor.x * t;
            float g = theme.backgroundBottomColor.y * (1 - t) + theme.backgroundTopColor.y * t;
            float b = theme.backgroundBottomColor.z * (1 - t) + theme.backgroundTopColor.z * t;
            uint8_t cr = (Uint8)(r * 255);
            uint8_t cg = (Uint8)(g * 255);
            uint8_t cb = (Uint8)(b * 255);
            uint32_t color = SDL_MapRGBA(details, nullptr, cr, cg, cb, 255);
            
            for (int x = 0; x < m_config.thumbnailSize; ++x) {
                pixels[y * pitch + x] = color;
            }
        }
        
        SDL_UnlockSurface(surface);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        SDL_DestroySurface(surface);
        return texture;
    }

    SDL_Texture* ThemeSelector::loadTexture(const std::string& path) {
        (void)path;
        return nullptr;
    }

    std::string ThemeSelector::formatThemeDescription(const Themes::Theme& theme) {
        std::string desc = theme.description;
        if (desc.length() > 100) desc = desc.substr(0, 97) + "...";
        return desc;
    }

    int ThemeSelector::panelY_start() const {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        return (winH - m_config.panelHeight) / 2;
    }

} // namespace ShapeEngine::UI
