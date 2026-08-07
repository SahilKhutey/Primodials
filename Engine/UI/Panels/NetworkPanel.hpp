#pragma once

#include "UI/IPanel.hpp"
#include "Network/NetworkManager.hpp"

namespace Shape::UI {

class NetworkPanel final : public IPanel {
public:
    explicit NetworkPanel(Network::NetworkManager* networkManager);
    ~NetworkPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Network Session"; }

private:
    Network::NetworkManager* m_networkManager = nullptr;
};

} // namespace Shape::UI
