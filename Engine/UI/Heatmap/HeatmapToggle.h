// Engine/UI/Heatmap/HeatmapToggle.h
#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace ShapeEngine::UI {

    enum class HeatmapMode {
        None,
        PopulationDensity,
        EnergyDistribution,
        GeneMutationRate,
        PheromoneTrails,
        Temperature
    };

    struct HeatmapColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        HeatmapColorRGBA() = default;
        HeatmapColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// Heatmap mode switcher and legend overlay for ecosystem visualization.
    /// Toggle with H key.
    /// </summary>
    class HeatmapToggle {
    public:
        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            HeatmapColorRGBA backgroundColor = {20, 24, 32, 230};
            HeatmapColorRGBA textColor = {230, 235, 245, 255};
            HeatmapColorRGBA accentColor = {100, 180, 255, 255};
            HeatmapColorRGBA borderColor = {60, 70, 85, 255};
        };

        explicit HeatmapToggle(const Config& config);
        ~HeatmapToggle();

        void render();
        void update(float deltaTime);
        bool handleInput();

        void setMode(HeatmapMode mode) { m_currentMode = mode; }
        HeatmapMode getMode() const { return m_currentMode; }

        void showLegend() { m_showLegend = true; }
        void hideLegend() { m_showLegend = false; }
        void toggleLegend() { m_showLegend = !m_showLegend; }
        bool isLegendVisible() const { return m_showLegend; }

        std::string getModeName(HeatmapMode mode) const;

    private:
        void renderModeSelector();
        void renderLegend();
        void renderText(const std::string& text, int x, int y, int size, 
                         HeatmapColorRGBA color, bool centered = false);

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        HeatmapMode m_currentMode = HeatmapMode::None;
        bool m_showLegend = false;
    };

} // namespace ShapeEngine::UI
