#pragma once

#include "UI/IPanel.hpp"
#include <vector>

namespace Shape::UI {

struct SpeciesTraits {
    u32 speciesId = 0;
    f32 speed = 1.0f;
    f32 size = 1.0f;
    f32 vision = 50.0f;
    f32 aggression = 0.1f;
    f32 defense = 0.5f;
    f32 metabolism = 1.0f;
};

class SpeciesEditorPanel final : public IPanel {
public:
    SpeciesEditorPanel();
    ~SpeciesEditorPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Species Genome Editor"; }

    SpeciesTraits& GetActiveTraits() { return m_activeTraits; }
    const SpeciesTraits& GetActiveTraits() const { return m_activeTraits; }

private:
    SpeciesTraits m_activeTraits;
};

} // namespace Shape::UI
