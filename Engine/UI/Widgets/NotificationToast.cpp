// Engine/UI/Widgets/NotificationToast.cpp
#include "NotificationToast.h"
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

    NotificationToast::NotificationToast(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
    }

    NotificationToast::~NotificationToast() = default;

    void NotificationToast::show(const std::string& title, 
                                    const std::string& message, 
                                    ToastType type, float duration) {
        Toast toast;
        toast.title = title;
        toast.message = message;
        toast.type = type;
        toast.duration = (duration > 0) ? duration : m_config.defaultDuration;
        toast.timeRemaining = toast.duration;
        toast.color = getColorForType(type);
        toast.timestamp = SDL_GetTicks();
        
        m_toasts.push_back(toast);
        
        while ((int)m_toasts.size() > m_config.maxVisible) {
            m_toasts.pop_front();
        }
        
        SHAPE_LOG_INFO("Toast shown: [%s] %s", title.c_str(), message.c_str());
    }

    void NotificationToast::clear() {
        m_toasts.clear();
    }

    void NotificationToast::update(float deltaTime) {
        for (auto& toast : m_toasts) {
            toast.timeRemaining -= deltaTime;
        }
        
        while (!m_toasts.empty() && m_toasts.front().timeRemaining < -m_config.fadeOutDuration) {
            m_toasts.pop_front();
        }
    }

    void NotificationToast::render() {
        if (!m_renderer) return;
        
        int index = 0;
        for (const auto& toast : m_toasts) {
            float lifeT = 1.0f - (toast.timeRemaining / toast.duration);
            float slideProgress = 1.0f;
            
            if (lifeT < (m_config.slideInDuration / toast.duration)) {
                slideProgress = lifeT / (m_config.slideInDuration / toast.duration);
            }
            
            renderToast(index, toast, slideProgress);
            index++;
        }
    }

    void NotificationToast::renderToast(int index, const Toast& toast, 
                                          float slideProgress) {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        float slideOffset = (1.0f - slideProgress) * (m_config.toastWidth + m_config.margin);
        int x = (int)(winW - m_config.toastWidth - m_config.margin + slideOffset);
        int y = (int)(m_config.margin + index * (m_config.toastHeight + m_config.stackOffset));
        
        SDL_FRect toastRect = {(float)x, (float)y, (float)m_config.toastWidth, (float)m_config.toastHeight};
        
        float alpha = 1.0f;
        if (toast.timeRemaining < m_config.fadeOutDuration) {
            alpha = std::clamp(toast.timeRemaining / m_config.fadeOutDuration, 0.0f, 1.0f);
        }
        
        ToastColorRGBA bgColor = toast.color;
        bgColor.a = (Uint8)(bgColor.a * alpha);
        
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, (Uint8)(80 * alpha));
        SDL_FRect shadow = {(float)(x + 3), (float)(y + 3), 
                            (float)m_config.toastWidth, (float)m_config.toastHeight};
        SDL_RenderFillRect(m_renderer, &shadow);
        
        SDL_SetRenderDrawColor(m_renderer, 25, 28, 35, (Uint8)(240 * alpha));
        SDL_RenderFillRect(m_renderer, &toastRect);
        
        SDL_FRect stripe = {(float)x, (float)y, 5.0f, (float)m_config.toastHeight};
        SDL_SetRenderDrawColor(m_renderer, bgColor.r, bgColor.g, bgColor.b, (Uint8)(255 * alpha));
        SDL_RenderFillRect(m_renderer, &stripe);
        
        SDL_Rect iconRect = {x + 12, y + 15, 40, 40};
        renderIcon(iconRect, toast.type);
        
        ToastColorRGBA titleColor = {240, 245, 255, (Uint8)(255 * alpha)};
        renderText(toast.title, x + 60, y + 10, 14, titleColor);
        
        ToastColorRGBA msgColor = {200, 210, 220, (Uint8)(220 * alpha)};
        renderText(toast.message, x + 60, y + 30, 12, msgColor);
        
        float progress = std::clamp(toast.timeRemaining / toast.duration, 0.0f, 1.0f);
        SDL_FRect timeBar = {(float)(x + 5), (float)(y + m_config.toastHeight - 3), 
                            (float)((m_config.toastWidth - 10) * progress), 3.0f};
        SDL_SetRenderDrawColor(m_renderer, bgColor.r, bgColor.g, bgColor.b, 
                                (Uint8)(150 * alpha));
        SDL_RenderFillRect(m_renderer, &timeBar);
    }

    void NotificationToast::renderIcon(SDL_Rect iconRect, ToastType type) {
        ToastColorRGBA iconColor;
        const char* letter = "i";
        
        switch (type) {
            case ToastType::Info:    iconColor = {100, 180, 230, 255}; letter = "i"; break;
            case ToastType::Success: iconColor = {120, 220, 140, 255}; letter = "V"; break;
            case ToastType::Warning: iconColor = {240, 190, 80, 255};  letter = "!"; break;
            case ToastType::Error:   iconColor = {230, 90, 90, 255};   letter = "X"; break;
            case ToastType::Achievement: iconColor = {200, 150, 255, 255}; letter = "*"; break;
            case ToastType::AchievementHidden: iconColor = {150, 100, 200, 255}; letter = "?"; break;
        }
        
        SDL_SetRenderDrawColor(m_renderer, iconColor.r, iconColor.g, iconColor.b, 255);
        drawCircle(iconRect.x + iconRect.w / 2, iconRect.y + iconRect.h / 2, 
                   iconRect.w / 2, 24);
        
        renderText(letter, iconRect.x + iconRect.w / 2, 
                   iconRect.y + iconRect.h / 2 - 6, 16, {255, 255, 255, 255}, true);
    }

    void NotificationToast::drawCircle(int cx, int cy, int radius, int segments) {
        if (!m_renderer) return;
        std::vector<SDL_FPoint> points;
        for (int i = 0; i <= segments; ++i) {
            float angle = (i / (float)segments) * 6.2831853f;
            points.push_back({(float)(cx + std::cos(angle) * radius), 
                              (float)(cy + std::sin(angle) * radius)});
        }
        SDL_RenderLines(m_renderer, points.data(), (int)points.size());
    }

    ToastColorRGBA NotificationToast::getColorForType(ToastType type) {
        switch (type) {
            case ToastType::Info: return m_config.infoColor;
            case ToastType::Success: return m_config.successColor;
            case ToastType::Warning: return m_config.warningColor;
            case ToastType::Error: return m_config.errorColor;
            case ToastType::Achievement: return m_config.achievementColor;
            case ToastType::AchievementHidden: return m_config.achievementHiddenColor;
        }
        return m_config.infoColor;
    }

    void NotificationToast::renderText(const std::string& text, int x, int y, int size, 
                                          ToastColorRGBA color, bool centered) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        float drawX = centered ? (float)x - (text.length() * 4.0f) : (float)x;
        SDL_RenderDebugText(m_renderer, drawX, (float)y, text.c_str());
    }

} // namespace ShapeEngine::UI
