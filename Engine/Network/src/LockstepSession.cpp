// Shape/Network/LockstepSession.cpp
#include "Shape/Network/LockstepSession.h"
#include "Shape/Utility/CRC32.hpp"
#include "Shape/Core/Logger.hpp"

#include <algorithm>
#include <cstring>

namespace Shape::Network {

    LockstepSession::LockstepSession() = default;
    LockstepSession::~LockstepSession() { shutdown(); }

    bool LockstepSession::host(const LockstepConfig& cfg) {
        m_cfg = cfg;
        m_backend = create_backend("enet");
        if (!m_backend || !m_backend->initialize()) return false;
        if (!m_backend->start_server(cfg.port, cfg.max_players)) return false;
        m_local_player_id = 1;
        m_state = LockstepState::WaitingForPlayers;
        SHAPE_LOG_INFO("Net", "Hosting on port {}", cfg.port);
        return true;
    }

    bool LockstepSession::join(const std::string& address, uint16_t port) {
        m_backend = create_backend("enet");
        if (!m_backend || !m_backend->initialize()) return false;
        if (!m_backend->connect(address, port)) return false;
        m_state = LockstepState::WaitingForPlayers;
        SHAPE_LOG_INFO("Net", "Connecting to {}:{}", address, port);
        return true;
    }

    void LockstepSession::leave() {
        if (m_backend) m_backend->disconnect();
        m_state = LockstepState::Idle;
    }

    void LockstepSession::shutdown() {
        if (m_backend) {
            m_backend->shutdown();
            m_backend.reset();
        }
        m_state = LockstepState::Idle;
    }

    bool LockstepSession::is_host() const { return m_backend && m_backend->is_server(); }
    bool LockstepSession::is_running() const { return m_state == LockstepState::Running; }

    void LockstepSession::submit_local_input(const std::vector<uint8_t>& input) {
        m_local_input = input;
    }

    void LockstepSession::update(ECS::World& world, Simulation::SimulationScheduler& scheduler, double /*real_delta*/) {
        if (!m_backend) return;
        m_backend->poll();

        // Process received packets
        uint32_t peer = 0;
        Packet packet;
        while (m_backend->receive(peer, packet)) {
            on_packet(peer, packet);
        }

        // State machine
        switch (m_state) {
            case LockstepState::Idle:
                break;
            case LockstepState::WaitingForPlayers:
                if (m_backend->connected_count() > 0 && is_host()) {
                    // Send start signal
                    Packet p(PacketType::Welcome);
                    p.write(m_local_player_id);
                    p.write(static_cast<uint32_t>(m_backend->connected_count() + 1));
                    m_backend->broadcast(p);
                    m_state = LockstepState::Countdown;
                    m_countdown = m_cfg.countdown_ticks;
                }
                break;
            case LockstepState::Countdown:
                if (--m_countdown <= 0) {
                    m_state = LockstepState::Running;
                    SHAPE_LOG_INFO("Net", "Lockstep started, all players connected");
                }
                break;
            case LockstepState::Running:
                run_frame(world, scheduler);
                break;
            case LockstepState::Paused:
                break;
            case LockstepState::Finished:
                break;
            case LockstepState::Desynced:
                SHAPE_LOG_ERROR("Net", "DESYNC detected at tick {}!", scheduler.current_tick());
                break;
        }
    }

    void LockstepSession::run_frame(ECS::World& world, Simulation::SimulationScheduler& scheduler) {
        // 1) Send local input for next tick
        Packet input_pkt(PacketType::Input);
        input_pkt.write(scheduler.current_tick() + 1);
        input_pkt.write(m_local_player_id);
        input_pkt.set_body(m_local_input);
        m_backend->broadcast(input_pkt);

        // 2) Wait for all inputs
        collect_inputs(world, scheduler);

        // 3) Step simulation deterministically
        scheduler.step_once(world);

        // 4) Periodically check state hashes
        if (scheduler.current_tick() % m_cfg.state_hash_interval_ticks == 0) {
            check_state_hashes(world, scheduler.current_tick());
        }
    }

    void LockstepSession::collect_inputs(ECS::World& /*world*/, Simulation::SimulationScheduler& scheduler) {
        // For lockstep, we advance one tick only when all inputs are collected.
        // In a real impl, we'd block on inputs; here we just track that we have them.
        m_players.clear();
        const int count = m_backend->connected_count();
        for (int i = 0; i < count; ++i) {
            PlayerSlot p;
            p.connected = true;
            p.ready = true;
            m_players.push_back(p);
        }
        (void)scheduler;
    }

    void LockstepSession::broadcast_commands() {}

    void LockstepSession::check_state_hashes(ECS::World& world, Simulation::Tick tick) {
        const uint64_t h = hash_world_state(world);
        Packet p(PacketType::StateHash);
        p.write(tick);
        p.write(h);
        m_backend->broadcast(p);
    }

    void LockstepSession::send_state_hash(ECS::World& world, Simulation::Tick tick) {
        check_state_hashes(world, tick);
    }

    uint64_t LockstepSession::hash_world_state(ECS::World& world) const {
        // Hash a deterministic snapshot of all entity counts and one random position per entity
        uint64_t h = 0xCBF29CE484222325ull;
        h ^= Random::SplitMix64(world.stats().entity_count).next();
        h *= 0x100000001B3ull;
        // Note: in full impl, hash every entity's position, velocity, health etc.
        return h;
    }

    void LockstepSession::on_packet(uint32_t peer, const Packet& packet) {
        switch (packet.header().type) {
            case PacketType::Welcome: {
                size_t off = 0;
                const uint32_t my_id = packet.read<uint32_t>(off);
                (void)packet.read<uint32_t>(off);
                m_local_player_id = my_id;
                peer = peer;
                break;
            }
            case PacketType::Input: {
                size_t off = 0;
                const Shape::Simulation::Tick tick = packet.read<Shape::Simulation::Tick>(off);
                const uint32_t player = packet.read<uint32_t>(off);
                (void)tick; (void)player;
                // Store input for processing
                break;
            }
            case PacketType::StateHash: {
                size_t off = 0;
                const Shape::Simulation::Tick tick = packet.read<Shape::Simulation::Tick>(off);
                const uint64_t remote_hash = packet.read<uint64_t>(off);
                (void)tick;
                if (remote_hash != m_local_player_id) {
                    // Check for desync
                    // ...
                }
                break;
            }
            default: break;
        }
        (void)peer;
    }

    void LockstepSession::report_desync(Shape::Simulation::Tick tick, uint64_t local, uint64_t remote) {
        SHAPE_LOG_ERROR("Net", "DESYNC at tick {}: local={:X} remote={:X}", tick, local, remote);
        m_state = LockstepState::Desynced;
        ++m_desync_count;
    }

    LockstepSession::Stats LockstepSession::stats() const {
        Stats s;
        s.state = m_state;
        s.connected_players = m_backend ? m_backend->connected_count() : 0;
        s.current_tick = 0;
        s.desync_count = m_desync_count;
        return s;
    }

} // namespace Shape::Network
