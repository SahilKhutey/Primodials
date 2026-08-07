// Engine/UI/Widgets/ColorPicker.h
#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace ShapeEngine::UI {

    struct ColorPickerRGBA {
        float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
        ColorPickerRGBA() = default;
        ColorPickerRGBA(float r_, float g_, float b_, float a_ = 1.0f) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// RGB color picker with HSV support and hex input.
    /// Used in theme creator for custom colors.
    /// </summary>
    class ColorPicker {
    public:
        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            int pickerWidth = 200;
            int pickerHeight = 30;
            bool showAlpha = true;
            bool showHex = true;
            
            SDL_Color textColor = {230, 235, 245, 255};
            SDL_Color secondaryTextColor = {140, 150, 170, 255};
            SDL_Color accentColor = {100, 180, 255, 255};
            SDL_Color borderColor = {60, 70, 85, 255};
        };

        explicit ColorPicker(const Config& config);
        ~ColorPicker();

        bool render(int x, int y, ColorPickerRGBA& color, const std::string& label = "");
        void renderSwatch(int x, int y, int size, const ColorPickerRGBA& color);

    private:
        void renderHueSlider(int x, int y, int w, int h, float& hue);
        void renderSaturationValueSquare(int x, int y, int size, float hue);
        void renderAlphaSlider(int x, int y, int w, int h, float& alpha);
        void rgbToHsv(const ColorPickerRGBA& rgb, float& h, float& s, float& v);
        void hsvToRgb(float h, float s, float v, ColorPickerRGBA& rgb);

        Config m_config;
    };

} // namespace ShapeEngine::UI
