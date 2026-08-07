#include "UI/Panels/InspectorPanel.hpp"
#include "UI/ImGuiLayer.hpp"
#include "ECS/TransformComponent.hpp"

namespace Shape::UI {

void InspectorPanel::Draw(World& world) {
    Entity selected = ImGuiLayer::GetSelectedEntity();
    if (selected != INVALID_ENTITY && world.IsEntityValid(selected)) {
        if (world.HasComponent<TransformComponent>(selected)) {
            // Inspected transform data
        }
    }
}

} // namespace Shape::UI
