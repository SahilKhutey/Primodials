#include "UI/Panels/CommunityHubPanel.hpp"
#include "Steam/SteamContext.hpp"

namespace Shape {

CommunityHubPanel::CommunityHubPanel(Steam::SteamContext* context)
    : m_context(context) {
    m_leaderboards = std::make_unique<Community::LeaderboardClient>(context);
    m_friends = std::make_unique<Community::FriendsListManager>(context);
    m_screenshots = std::make_unique<Community::ScreenshotUploader>();
}

void CommunityHubPanel::Update(f32 dt) {
    (void)dt;
}

void CommunityHubPanel::Draw(World& world) {
    (void)world;
}

} // namespace Shape
