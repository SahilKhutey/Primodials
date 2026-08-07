#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <vector>
#include <array>

namespace Shape::Steam { class SteamContext; }

namespace Shape::Community {

enum class Board : u32 {
    LongestLineage = 0,
    MaxPopulation = 1,
    TotalPredation = 2,
    FirstExtinctionSpeedrun = 3,
    MostSpecies = 4,
    Count = 5
};

struct LeaderboardEntry {
    std::string playerName;
    u64 steamId = 0;
    i32 score = 0;
    i32 globalRank = 0;
    bool isLocalPlayer = false;
};

class LeaderboardClient {
public:
    explicit LeaderboardClient(Steam::SteamContext* context);
    ~LeaderboardClient() = default;

    bool SubmitScore(Board board, i32 score);
    void RequestTop(Board board, usize count = 50);
    const std::vector<LeaderboardEntry>& GetEntries(Board board) const;
    Steam::SteamContext* GetContext() const { return m_context; }

private:
    Steam::SteamContext* m_context = nullptr;
    std::array<std::vector<LeaderboardEntry>, static_cast<size_t>(Board::Count)> m_cache;
};

} // namespace Shape::Community
