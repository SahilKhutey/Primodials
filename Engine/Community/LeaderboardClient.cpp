#include "Community/LeaderboardClient.hpp"
#include "Steam/SteamContext.hpp"

namespace Shape::Community {

LeaderboardClient::LeaderboardClient(Steam::SteamContext* context)
    : m_context(context) {}

bool LeaderboardClient::SubmitScore(Board board, i32 score) {
    auto idx = static_cast<size_t>(board);
    if (idx < static_cast<size_t>(Board::Count)) {
        LeaderboardEntry entry;
        entry.playerName = m_context ? m_context->GetLocalUserName() : "Player";
        entry.steamId = m_context ? m_context->GetLocalUserId() : 0;
        entry.score = score;
        entry.globalRank = 1;
        entry.isLocalPlayer = true;
        m_cache[idx].clear();
        m_cache[idx].push_back(entry);
        return true;
    }
    return false;
}

void LeaderboardClient::RequestTop(Board board, usize count) {
    (void)count;
    auto idx = static_cast<size_t>(board);
    if (idx < static_cast<size_t>(Board::Count) && m_cache[idx].empty()) {
        LeaderboardEntry entry;
        entry.playerName = "TopPlayer";
        entry.score = 1000;
        entry.globalRank = 1;
        m_cache[idx].push_back(entry);
    }
}

const std::vector<LeaderboardEntry>& LeaderboardClient::GetEntries(Board board) const {
    auto idx = static_cast<size_t>(board);
    if (idx < static_cast<size_t>(Board::Count)) {
        return m_cache[idx];
    }
    static const std::vector<LeaderboardEntry> empty;
    return empty;
}

} // namespace Shape::Community
