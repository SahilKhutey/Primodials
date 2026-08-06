#pragma once

#include "ECS/World.hpp"

namespace Shape::UI {

class IPanel {
public:
    virtual ~IPanel() = default;
    virtual void Draw(World& world) = 0;
    virtual const char* GetName() const = 0;
};

} // namespace Shape::UI
