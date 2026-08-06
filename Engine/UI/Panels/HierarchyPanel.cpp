#include "UI/Panels/HierarchyPanel.hpp"
#include "UI/ImGuiLayer.hpp"
#include "ECS/TransformComponent.hpp"

namespace Shape::UI {

void HierarchyPanel::Draw(World& world) {
    auto entities = world.Query<TransformComponent>();
    if (!entities.empty()) {
        ImGuiLayer::SetSelectedEntity(entities[0]);
    }
}

} // namespace Shape::UI
