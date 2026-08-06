#pragma once

#include "UI/IPanel.hpp"
#include "Steam/Workshop/WorkshopManager.hpp"
#include <vector>

namespace Shape {

enum class WorkshopSortMode { Trending, TopRated, MostSubscribed, Newest };

class WorkshopBrowserPanel final : public UI::IPanel {
public:
    explicit WorkshopBrowserPanel(Steam::WorkshopManager* manager);
    ~WorkshopBrowserPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Workshop Browser"; }
    void Refresh();

    usize GetItemCount() const { return m_manager ? m_manager->GetAvailableItems().size() : 0; }
    WorkshopSortMode GetSortMode() const { return m_sortMode; }
    void SetSortMode(WorkshopSortMode mode) { m_sortMode = mode; }

private:
    Steam::WorkshopManager* m_manager = nullptr;
    WorkshopSortMode m_sortMode = WorkshopSortMode::Trending;
};

} // namespace Shape
