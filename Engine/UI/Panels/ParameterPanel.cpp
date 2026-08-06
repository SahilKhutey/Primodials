#include "UI/Panels/ParameterPanel.hpp"
#include "UI/ImGuiLayer.hpp"

namespace Shape::UI {

void ParameterPanel::Draw(World& world) {
    (void)world;
    auto& params = ImGuiLayer::GetSimParams();
    // Keep parameters bounded
    if (params.mutationRate < 0.0f) params.mutationRate = 0.0f;
    if (params.carryingCapacity < 10.0f) params.carryingCapacity = 10.0f;
}

} // namespace Shape::UI
