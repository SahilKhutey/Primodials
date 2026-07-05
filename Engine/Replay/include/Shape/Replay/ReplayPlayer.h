// Shape/Replay/ReplayPlayer.h
// Replay playback with timeline scrubbing
#pragma once

#include "Shape/Replay/ReplayFormat.h"

#include <cstdint>
#include <fstream>
#include <vector>
#include <memory>
#include <string>

namespace Shape::Replay {

    struct ReplayCommand {
        uint64_t tick;
        CommandType type;
        std::vector<uint8_t> data;
    };

    enum class PlayerState : uint8_t {
        Stopped,
        Playing,
        Paused,
        Finished,
        Seeking
    };

    class ReplayPlayer {
    public:
        ReplayPlayer();
        ~ReplayPlayer();

        bool open(const std::string& path);
        void close();

        bool is_open() const { return m_file.is_open(); }

        // Playback
        void play();
        void pause();
        void stop();
        void seek_to_tick(uint64_t tick);
        void seek_to_fraction(float t01);  // 0..1

        // Step
        bool step_forward();
        bool step_backward();

        // Update each frame
        void update(double real_delta);

        // State
        PlayerState state() const { return m_state; }
        uint64_t current_tick() const { return m_current_tick; }
        uint64_t total_ticks() const { return m_total_ticks; }
        float playback_position() const {
            return m_total_ticks > 0 ? static_cast<float>(m_current_tick) / static_cast<float>(m_total_ticks) : 0;
        }
        float playback_speed() const { return m_playback_speed; }
        void set_playback_speed(float s) { m_playback_speed = s; }

        // Read pending commands at current tick
        const std::vector<ReplayCommand>& pending_commands() const { return m_pending; }
        void clear_pending() { m_pending.clear(); }

        // Header
        uint64_t seed() const { return m_header.seed; }
        const std::vector<uint8_t>& world_state() const { return m_world_state; }

    private:
        bool read_next_command(ReplayCommand& cmd);
        bool read_header();
        void collect_commands_for_tick(uint64_t tick);

        std::ifstream m_file;
        ReplayHeader m_header{};
        std::vector<uint8_t> m_world_state;
        PlayerState m_state = PlayerState::Stopped;
        uint64_t m_current_tick = 0;
        uint64_t m_total_ticks = 0;
        float m_playback_speed = 1.0f;
        std::vector<ReplayCommand> m_pending;
        std::vector<ReplayCommand> m_buffer;
        size_t m_buffer_pos = 0;
    };

} // namespace Shape::Replay
