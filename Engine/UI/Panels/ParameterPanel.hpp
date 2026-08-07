#pragma once

#include "UI/IPanel.hpp"

namespace Shape::UI {

class ParameterPanel final : public IPanel {
public:
    ParameterPanel() = default;
    ~ParameterPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Simulation Parameters"; }
};

} // namespace Shape::UI
