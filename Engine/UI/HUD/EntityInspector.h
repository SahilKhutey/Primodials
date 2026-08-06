// Engine/UI/HUD/EntityInspector.h
#pragma once

#include "ECS/World.hpp"
#include "ECS/Entity.hpp"
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>

namespace ShapeEngine::UI {

    using EntityId = Shape::Entity;
    constexpr EntityId INVALID_ENTITY = Shape::INVALID_ENTITY;

    struct InspectorColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        InspectorColorRGBA() = default;
        InspectorColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    struct InspectorVec2 {
        float x = 0.0f, y = 0.0f;
        InspectorVec2() = default;
        InspectorVec2(float x_, float y_) : x(x_), y(y_) {}
    };

    /// <summary>
    /// Right-side panel showing detailed information about the selected entity.
    /// Displays genome radar chart, traits, history, and relationships.
    /// </summary>
    class EntityInspector {
    public:
        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            // Panel dimensions
            int panelWidth = 350;
            int minHeight = 400;
            
            // Sections
            bool showGenomeRadar = true;
            bool showTraitBars = true;
            bool showLineageTree = true;
            bool showRelationships = true;
            bool showHistory = true;
            
            // Styling
            InspectorColorRGBA backgroundColor = {20, 22, 28, 240};
            InspectorColorRGBA headerColor = {40, 60, 90, 255};
            InspectorColorRGBA sectionColor = {30, 35, 45, 255};
            InspectorColorRGBA textColor = {230, 235, 245, 255};
            InspectorColorRGBA secondaryTextColor = {140, 150, 170, 255};
            InspectorColorRGBA accentColor = {100, 180, 255, 255};
            InspectorColorRGBA warningColor = {255, 200, 100, 255};
            InspectorColorRGBA dangerColor = {255, 100, 100, 255};
            InspectorColorRGBA successColor = {130, 220, 150, 255};
            
            // Trait colors (8 traits)
            InspectorColorRGBA traitColors[8] = {
                {255, 150, 100, 255},  // Speed (orange)
                {100, 200, 255, 255},  // Size (blue)
                {200, 255, 100, 255},  // Vision (green)
                {255, 100, 100, 255},  // Aggression (red)
                {150, 100, 255, 255},  // Defense (purple)
                {255, 200, 100, 255},  // Metabolism (yellow)
                {100, 255, 200, 255},  // Reproduction (cyan)
                {200, 100, 200, 255}   // Lifespan (magenta)
            };
        };

        explicit EntityInspector(const Config& config);
        ~EntityInspector();

        void render();
        void update(float deltaTime);
        bool handleInput();

        // ─── Selection ─────────────────────────────────────────
        void setSelectedEntity(EntityId entity);
        EntityId getSelectedEntity() const { return m_selectedEntity; }
        void clearSelection() { m_selectedEntity = INVALID_ENTITY; }

        // ─── Visibility ───────────────────────────────────────
        void show() { m_visible = true; }
        void hide() { m_visible = false; m_selectedEntity = INVALID_ENTITY; }
        void toggle() { m_visible = !m_visible; }
        bool isVisible() const { return m_visible; }

        // ─── External close callback ──────────────────────────
        using CloseCallback = std::function<void()>;
        void setOnClose(CloseCallback cb) { m_closeCallback = std::move(cb); }
        using ActionCallback = std::function<void(const std::string& action, 
                                                     EntityId entity)>;
        void setOnAction(ActionCallback cb) { m_actionCallback = std::move(cb); }

    private:
        void renderHeader();
        void renderGenomeRadar();
        void renderTraitBars();
        void renderLineageInfo(int& y);
        void renderRelationships(int& y);
        void renderHistory(int& y);
        void renderActionButtons();
        
        void renderText(const std::string& text, int x, int y, int size, 
                         InspectorColorRGBA color, bool centered = false);
        bool renderSection(const std::string& title, int& y);
        void renderCloseButton();
        void drawCircle(int cx, int cy, int radius, int segments);
        bool renderButton(const std::string& label, SDL_Rect rect);
        
        void renderRadarChart(int cx, int cy, int radius, 
                                const float traits[8], 
                                const InspectorColorRGBA colors[8]);
        
        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_visible = false;
        EntityId m_selectedEntity = INVALID_ENTITY;
        
        struct CachedData {
            bool isAlive = true;
            float traits[8] = {0.7f, 0.5f, 0.8f, 0.4f, 0.6f, 0.5f, 0.9f, 0.7f};
            int generation = 12;
            uint32_t speciesId = 1;
            float energy = 85.0f, energyMax = 100.0f;
            float age = 45.0f;
            InspectorVec2 position = {100.0f, 200.0f};
            float velocity = 2.5f;
            std::vector<std::string> parents = {"Entity #102", "Entity #104"};
            int childrenCount = 3;
            float timeAlive = 45.0f;
            int kills = 2;
        } m_cached;
        
        int m_scrollOffset = 0;
        int m_maxScroll = 0;
        
        CloseCallback m_closeCallback;
        ActionCallback m_actionCallback;
    };

} // namespace ShapeEngine::UI
