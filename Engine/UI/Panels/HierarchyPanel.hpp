#pragma once

#include "UI/IPanel.hpp"
#include <vector>
#include <string>

namespace Shape::UI {

class HierarchyPanel final : public IPanel {
public:
    HierarchyPanel() = default;
    ~HierarchyPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Entity Hierarchy"; }

    void SetSearchFilter(const std::string& filter) { m_searchFilter = filter; }
    const std::string& GetSearchFilter() const { return m_searchFilter; }

private:
    std::string m_searchFilter;
};

} // namespace Shape::UI
