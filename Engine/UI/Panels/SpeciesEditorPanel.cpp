#include "UI/Panels/SpeciesEditorPanel.hpp"
#include "UI/ImGuiLayer.hpp"
#include "ECS/TransformComponent.hpp"

namespace Shape::UI {

SpeciesEditorPanel::SpeciesEditorPanel() {}

void SpeciesEditorPanel::Draw(World& world) {
    (void)world;
    i32 speciesId = static_cast<i32>(m_activeTraits.speciesId);
    m_activeTraits.speciesId = static_cast<u32>(speciesId);
}

} // namespace Shape::UI
