#pragma once

#include "UI/IPanel.hpp"
#include "ECS/World.hpp"
#include <vector>
#include <memory>

namespace Shape::UI {

struct SimParams {
    f32 mutationRate = 0.05f;
    f32 foodSpawnRate = 1.0f;
    f32 carryingCapacity = 5000.0f;
    f32 speedMultiplier = 1.0f;
    bool isPaused = false;
};

class ImGuiLayer {
public:
    ImGuiLayer() = default;
    ~ImGuiLayer() = default;

    void AddPanel(std::unique_ptr<IPanel> panel);
    void DrawAllPanels(World& world);

    static void SetSelectedEntity(Entity entity) { s_selectedEntity = entity; }
    static Entity GetSelectedEntity() { return s_selectedEntity; }

    static SimParams& GetSimParams() { return s_simParams; }

private:
    std::vector<std::unique_ptr<IPanel>> m_panels;
    static Entity s_selectedEntity;
    static SimParams s_simParams;
};

} // namespace Shape::UI
