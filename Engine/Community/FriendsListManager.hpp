#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <vector>

namespace Shape::Steam { class SteamContext; }

namespace Shape::Community {

struct FriendInfo {
    u64 steamId = 0;
    std::string name;
    std::string status = "Offline";
    bool isPlayingThisGame = false;
};

class FriendsListManager {
public:
    explicit FriendsListManager(Steam::SteamContext* context);
    ~FriendsListManager() = default;

    void Refresh();
    std::vector<FriendInfo> GetPlayingFriends() const;
    std::vector<FriendInfo> GetAllFriends() const;
    bool InviteToGame(u64 steamId);

    Steam::SteamContext* GetContext() const { return m_context; }

private:
    Steam::SteamContext* m_context = nullptr;
    std::vector<FriendInfo> m_friends;
};

} // namespace Shape::Community
