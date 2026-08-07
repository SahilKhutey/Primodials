#pragma once

#include "UI/IPanel.hpp"

namespace Shape::UI {

class InspectorPanel final : public IPanel {
public:
    InspectorPanel() = default;
    ~InspectorPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Entity Inspector"; }
};

} // namespace Shape::UI
