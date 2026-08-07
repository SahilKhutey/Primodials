// Engine/UI/Widgets/ColorPicker.cpp
#include "ColorPicker.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace ShapeEngine::UI {

    ColorPicker::ColorPicker(const Config& config) 
        : m_config(config) {
    }

    ColorPicker::~ColorPicker() = default;

    bool ColorPicker::render(int x, int y, ColorPickerRGBA& color, const std::string& label) {
        if (!m_config.renderer) return false;
        bool changed = false;
        
        int currentY = y;
        
        if (!label.empty()) {
            SDL_SetRenderDrawColor(m_config.renderer, m_config.textColor.r, m_config.textColor.g, m_config.textColor.b, 255);
            SDL_RenderDebugText(m_config.renderer, (float)x, (float)currentY, label.c_str());
            currentY += 20;
        }
        
        float h = 0.0f, s = 0.0f, v = 0.0f;
        rgbToHsv(color, h, s, v);
        
        int svSize = m_config.pickerWidth - 60;
        renderSaturationValueSquare(x, currentY, svSize, h);
        
        renderHueSlider(x + svSize + 10, currentY, 20, svSize, h);
        currentY += svSize + 10;
        
        if (m_config.showAlpha) {
            renderAlphaSlider(x, currentY, m_config.pickerWidth, 15, color.a);
            currentY += 20;
        }
        
        if (m_config.showHex) {
            char hexStr[16];
            snprintf(hexStr, sizeof(hexStr), "#%02X%02X%02X",
                     (int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255));
            
            SDL_FRect hexBox = {(float)x, (float)currentY, (float)m_config.pickerWidth, 20.0f};
            SDL_SetRenderDrawColor(m_config.renderer, 20, 22, 28, 255);
            SDL_RenderFillRect(m_config.renderer, &hexBox);
            SDL_SetRenderDrawColor(m_config.renderer, m_config.borderColor.r,
                                     m_config.borderColor.g, 
                                     m_config.borderColor.b, 255);
            SDL_RenderRect(m_config.renderer, &hexBox);
            
            SDL_SetRenderDrawColor(m_config.renderer, m_config.textColor.r, m_config.textColor.g, m_config.textColor.b, 255);
            SDL_RenderDebugText(m_config.renderer, (float)(x + 5), (float)(currentY + 2), hexStr);
        }
        
        return changed;
    }

    void ColorPicker::renderSwatch(int x, int y, int size, const ColorPickerRGBA& color) {
        if (!m_config.renderer) return;
        SDL_FRect rect = {(float)x, (float)y, (float)size, (float)size};
        SDL_SetRenderDrawColor(m_config.renderer, 
            (Uint8)(color.r * 255), (Uint8)(color.g * 255),
            (Uint8)(color.b * 255), (Uint8)(color.a * 255));
        SDL_RenderFillRect(m_config.renderer, &rect);
        SDL_SetRenderDrawColor(m_config.renderer, 100, 100, 110, 255);
        SDL_RenderRect(m_config.renderer, &rect);
    }

    void ColorPicker::renderHueSlider(int x, int y, int w, int h, float& hue) {
        if (!m_config.renderer) return;
        for (int i = 0; i < h; ++i) {
            float t = (float)i / (float)h;
            ColorPickerRGBA color;
            hsvToRgb(t * 360.0f, 1.0f, 1.0f, color);
            SDL_SetRenderDrawColor(m_config.renderer,
                (Uint8)(color.r * 255), (Uint8)(color.g * 255),
                (Uint8)(color.b * 255), 255);
            SDL_RenderLine(m_config.renderer, (float)x, (float)(y + i), (float)(x + w), (float)(y + i));
        }
        
        int hueY = y + (int)(hue / 360.0f * h);
        SDL_SetRenderDrawColor(m_config.renderer, 255, 255, 255, 255);
        SDL_FRect indicator = {(float)(x - 2), (float)(hueY - 2), (float)(w + 4), 4.0f};
        SDL_RenderRect(m_config.renderer, &indicator);
    }

    void ColorPicker::renderSaturationValueSquare(int x, int y, int size, float hue) {
        if (!m_config.renderer) return;
        for (int py = 0; py < size; py += 2) {
            for (int px = 0; px < size; px += 2) {
                float s = (float)px / (float)size;
                float v = 1.0f - (float)py / (float)size;
                ColorPickerRGBA color;
                hsvToRgb(hue, s, v, color);
                SDL_SetRenderDrawColor(m_config.renderer,
                    (Uint8)(color.r * 255), (Uint8)(color.g * 255),
                    (Uint8)(color.b * 255), 255);
                SDL_FRect pixel = {(float)(x + px), (float)(y + py), 2.0f, 2.0f};
                SDL_RenderFillRect(m_config.renderer, &pixel);
            }
        }
        
        SDL_SetRenderDrawColor(m_config.renderer, 80, 80, 90, 255);
        SDL_FRect border = {(float)x, (float)y, (float)size, (float)size};
        SDL_RenderRect(m_config.renderer, &border);
    }

    void ColorPicker::renderAlphaSlider(int x, int y, int w, int h, float& alpha) {
        if (!m_config.renderer) return;
        int checkerSize = 4;
        for (int cy = y; cy < y + h; cy += checkerSize) {
            for (int cx = x; cx < x + w; cx += checkerSize) {
                bool dark = ((cx / checkerSize + cy / checkerSize) % 2) == 0;
                SDL_SetRenderDrawColor(m_config.renderer, 
                    dark ? 40 : 60, dark ? 40 : 60, dark ? 40 : 60, 255);
                SDL_FRect checker = {(float)cx, (float)cy, (float)checkerSize, (float)checkerSize};
                SDL_RenderFillRect(m_config.renderer, &checker);
            }
        }
        
        for (int px = 0; px < w; ++px) {
            float a = (float)px / (float)w;
            SDL_SetRenderDrawColor(m_config.renderer, 255, 255, 255, 
                                    (Uint8)(a * 255));
            SDL_RenderLine(m_config.renderer, (float)(x + px), (float)y, (float)(x + px), (float)(y + h));
        }
        
        int alphaX = x + (int)(alpha * w);
        SDL_SetRenderDrawColor(m_config.renderer, 255, 255, 255, 255);
        SDL_FRect indicator = {(float)(alphaX - 2), (float)(y - 2), 4.0f, (float)(h + 4)};
        SDL_RenderRect(m_config.renderer, &indicator);
    }

    void ColorPicker::rgbToHsv(const ColorPickerRGBA& rgb, float& h, float& s, float& v) {
        float r = rgb.r, g = rgb.g, b = rgb.b;
        float maxVal = std::max({r, g, b});
        float minVal = std::min({r, g, b});
        v = maxVal;
        float delta = maxVal - minVal;
        if (delta < 0.00001f) {
            h = 0; s = 0;
        } else {
            s = delta / maxVal;
            if (r == maxVal) h = (g - b) / delta;
            else if (g == maxVal) h = 2 + (b - r) / delta;
            else h = 4 + (r - g) / delta;
            h *= 60;
            if (h < 0) h += 360;
        }
    }

    void ColorPicker::hsvToRgb(float h, float s, float v, ColorPickerRGBA& rgb) {
        if (s < 0.001f) {
            rgb.r = rgb.g = rgb.b = v;
            return;
        }
        h = fmod(h, 360.0f) / 60.0f;
        int i = (int)floor(h);
        float f = h - i;
        float p = v * (1 - s);
        float q = v * (1 - s * f);
        float t = v * (1 - s * (1 - f));
        switch (i) {
            case 0: rgb.r = v; rgb.g = t; rgb.b = p; break;
            case 1: rgb.r = q; rgb.g = v; rgb.b = p; break;
            case 2: rgb.r = p; rgb.g = v; rgb.b = t; break;
            case 3: rgb.r = p; rgb.g = q; rgb.b = v; break;
            case 4: rgb.r = t; rgb.g = p; rgb.b = v; break;
            default: rgb.r = v; rgb.g = p; rgb.b = q; break;
        }
    }

} // namespace ShapeEngine::UI
