#pragma once

#include "UI/IPanel.hpp"
#include "Community/LeaderboardClient.hpp"
#include "Community/FriendsListManager.hpp"
#include "Community/ScreenshotUploader.hpp"
#include <memory>

namespace Shape::Steam { class SteamContext; }

namespace Shape {

class CommunityHubPanel final : public UI::IPanel {
public:
    explicit CommunityHubPanel(Steam::SteamContext* context);
    ~CommunityHubPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Community Hub"; }
    void Update(f32 dt);

    Community::LeaderboardClient* GetLeaderboardClient() { return m_leaderboards.get(); }
    Community::FriendsListManager* GetFriendsListManager() { return m_friends.get(); }
    Steam::SteamContext* GetContext() const { return m_context; }

private:
    Steam::SteamContext* m_context = nullptr;
    std::unique_ptr<Community::LeaderboardClient> m_leaderboards;
    std::unique_ptr<Community::FriendsListManager> m_friends;
    std::unique_ptr<Community::ScreenshotUploader> m_screenshots;
};

} // namespace Shape
