// Shape/Replay/ReplayFormat.h
#pragma once
#include <cstdint>
#include <vector>
#include <cstring>

namespace Shape::Replay {

    constexpr uint32_t REPLAY_MAGIC = 0x53485250;  // 'SHRP'
    constexpr uint32_t REPLAY_VERSION = 1;

    enum class CommandType : uint8_t {
        CreateEntity   = 1,
        DestroyEntity  = 2,
        SetComponent   = 3,
        AddComponent   = 4,
        RemoveComponent = 5,
        SetVelocity    = 10,
        Attack         = 20,
        Birth          = 21,
        Death          = 22,
        Reproduction   = 23,
        RNGSeed        = 30
    };

    struct CommandHeader {
        uint64_t tick;
        CommandType type;
        uint32_t size;  // Body size
    };

    struct ReplayHeader {
        uint32_t magic;
        uint32_t version;
        uint64_t seed;
        uint64_t start_time;
        uint64_t world_size_bytes;
        uint32_t expected_ticks;
        uint32_t reserved;
    };

    struct FileFooter {
        uint64_t total_ticks;
        uint64_t total_commands;
        uint32_t checksum;
        uint32_t reserved;
    };

} // namespace Shape::Replay
