// Shape/Network/LockstepSession.h
// Deterministic lockstep multiplayer
#pragma once

#include "Shape/Network/INetworkBackend.h"
#include "Shape/Network/NetPacket.h"
#include "Simulation/SimulationScheduler.hpp"
#include "ECS/World.hpp"
#include "Simulation/DeterministicRng.hpp"

#include <cstdint>
#include <vector>
#include <memory>
#include <string>

namespace Shape::Network {

    struct PlayerSlot {
        uint32_t id = 0;
        bool connected = false;
        bool ready = false;
        std::string name;
        std::vector<uint8_t> last_input;
        bool has_input_for_tick = false;
    };

    enum class LockstepState : uint8_t {
        Idle,
        WaitingForPlayers,
        Countdown,
        Running,
        Paused,
        Finished,
        Desynced
    };

    struct LockstepConfig {
        uint16_t port = 7777;
        int max_players = 4;
        int countdown_ticks = 180;  // 3 seconds
        int heartbeat_interval_ticks = 60;
        int state_hash_interval_ticks = 60;
        int max_rollback_ticks = 600;
    };

    class LockstepSession {
    public:
        LockstepSession();
        ~LockstepSession();

        bool host(const LockstepConfig& cfg = {});
        bool join(const std::string& address, uint16_t port);
        void leave();
        void shutdown();

        bool is_host() const;
        bool is_running() const;
        LockstepState state() const { return m_state; }

        // Per-frame update
        void update(World& world, SimulationScheduler& scheduler, double real_delta);

        // Submit local input
        void submit_local_input(const std::vector<uint8_t>& input);

        // Desync handling
        void report_desync(u64 tick, uint64_t local_hash, uint64_t remote_hash);

        // Stats
        struct Stats {
            LockstepState state = LockstepState::Idle;
            int connected_players = 0;
            int ready_players = 0;
            u64 current_tick = 0;
            double avg_latency_ms = 0;
            uint64_t desync_count = 0;
        };
        Stats stats() const;

    private:
        void run_frame(World& world, SimulationScheduler& scheduler);
        void collect_inputs(World& world, SimulationScheduler& scheduler);
        void broadcast_commands();
        void check_state_hashes(World& world, u64 tick);
        void send_state_hash(World& world, u64 tick);
        void on_packet(uint32_t peer, const Packet& packet);
        uint64_t hash_world_state(World& world) const;

        std::unique_ptr<INetworkBackend> m_backend;
        LockstepConfig m_cfg;
        LockstepState m_state = LockstepState::Idle;
        int m_countdown = 0;
        std::vector<PlayerSlot> m_players;
        uint32_t m_local_player_id = 0;
        std::vector<uint8_t> m_local_input;
        std::vector<std::vector<uint8_t>> m_tick_inputs;
        uint32_t m_seq_counter = 0;
        u64 m_last_state_hash_tick = 0;
        double m_total_latency = 0;
        uint64_t m_desync_count = 0;
    };

} // namespace Shape::Network
