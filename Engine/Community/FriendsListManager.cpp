#include "Community/FriendsListManager.hpp"
#include "Steam/SteamContext.hpp"

namespace Shape::Community {

FriendsListManager::FriendsListManager(Steam::SteamContext* context)
    : m_context(context) {
    Refresh();
}

void FriendsListManager::Refresh() {
    m_friends.clear();
    FriendInfo f1;
    f1.steamId = 76561198000000001ULL;
    f1.name = "CyberGenesis";
    f1.status = "Watching Ecosystem";
    f1.isPlayingThisGame = true;

    FriendInfo f2;
    f2.steamId = 76561198000000002ULL;
    f2.name = "GenomeGuru";
    f2.status = "In Menu";
    f2.isPlayingThisGame = true;

    m_friends.push_back(f1);
    m_friends.push_back(f2);
}

std::vector<FriendInfo> FriendsListManager::GetPlayingFriends() const {
    std::vector<FriendInfo> result;
    for (const auto& f : m_friends) {
        if (f.isPlayingThisGame) result.push_back(f);
    }
    return result;
}

std::vector<FriendInfo> FriendsListManager::GetAllFriends() const {
    return m_friends;
}

bool FriendsListManager::InviteToGame(u64 steamId) {
    (void)steamId;
    return true;
}

} // namespace Shape::Community
