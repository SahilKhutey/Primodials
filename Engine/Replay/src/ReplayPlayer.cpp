// Shape/Replay/ReplayPlayer.cpp
#include "Shape/Replay/ReplayPlayer.h"
#include "Shape/Core/Logger.hpp"

#include <algorithm>
#include <cstring>

namespace Shape::Replay {

    ReplayPlayer::ReplayPlayer() = default;
    ReplayPlayer::~ReplayPlayer() { close(); }

    bool ReplayPlayer::open(const std::string& path) {
        close();
        m_file.open(path, std::ios::binary | std::ios::in | std::ios::ate);
        if (!m_file) {
            SHAPE_LOG_ERROR("Replay", "Failed to open: {}", path.c_str());
            return false;
        }
        const std::streamsize size = m_file.tellg();
        m_file.seekg(0);

        if (!read_header()) {
            close();
            return false;
        }

        m_world_state.clear();
        while (m_file.good() && m_file.tellg() < size - static_cast<std::streamoff>(sizeof(FileFooter))) {
            CommandHeader ch;
            const auto pos = m_file.tellg();
            m_file.read(reinterpret_cast<char*>(&ch), sizeof(ch));
            if (!m_file.good() || ch.tick == 0 || ch.type == CommandType::CreateEntity) {
                m_file.seekg(pos);
                break;
            }
            m_file.ignore(ch.size);
        }

        m_file.seekg(size - sizeof(FileFooter));
        FileFooter footer;
        m_file.read(reinterpret_cast<char*>(&footer), sizeof(footer));
        m_total_ticks = footer.total_ticks;

        m_file.seekg(sizeof(ReplayHeader));
        m_state = PlayerState::Stopped;
        m_current_tick = 0;
        m_pending.clear();
        m_buffer.clear();
        m_buffer_pos = 0;

        return true;
    }

    void ReplayPlayer::close() {
        if (m_file.is_open()) m_file.close();
        m_pending.clear();
        m_buffer.clear();
        m_buffer_pos = 0;
        m_state = PlayerState::Stopped;
        m_current_tick = 0;
    }

    bool ReplayPlayer::read_header() {
        m_file.read(reinterpret_cast<char*>(&m_header), sizeof(m_header));
        if (!m_file) return false;
        if (m_header.magic != REPLAY_MAGIC || m_header.version != REPLAY_VERSION) return false;
        return true;
    }

    bool ReplayPlayer::read_next_command(ReplayCommand& cmd) {
        if (!m_file.is_open()) return false;
        CommandHeader ch;
        if (!m_file.read(reinterpret_cast<char*>(&ch), sizeof(ch))) return false;
        cmd.tick = ch.tick;
        cmd.type = ch.type;
        cmd.data.resize(ch.size);
        if (ch.size > 0) m_file.read(reinterpret_cast<char*>(cmd.data.data()), ch.size);
        return m_file.good();
    }

    void ReplayPlayer::collect_commands_for_tick(uint64_t tick) {
        m_pending.clear();
        while (m_buffer_pos < m_buffer.size() && m_buffer[m_buffer_pos].tick == tick) {
            m_pending.push_back(std::move(m_buffer[m_buffer_pos++]));
        }
    }

    void ReplayPlayer::play() {
        if (m_state == PlayerState::Finished) seek_to_tick(0);
        m_state = PlayerState::Playing;
    }

    void ReplayPlayer::pause() { if (m_state == PlayerState::Playing) m_state = PlayerState::Paused; }
    void ReplayPlayer::stop() { m_state = PlayerState::Stopped; m_current_tick = 0; }

    void ReplayPlayer::seek_to_tick(uint64_t tick) {
        m_state = PlayerState::Seeking;
        m_current_tick = std::min(tick, m_total_ticks);
        m_buffer.clear();
        m_buffer_pos = 0;
        m_pending.clear();

        m_file.clear();
        m_file.seekg(sizeof(ReplayHeader));
        while (m_file.good()) {
            ReplayCommand cmd;
            if (!read_next_command(cmd)) break;
            if (cmd.tick > m_current_tick) {
                const auto back = static_cast<std::streamoff>(sizeof(CommandHeader) + cmd.data.size());
                m_file.seekg(-back, std::ios::cur);
                break;
            }
            m_buffer.push_back(std::move(cmd));
        }
        m_buffer_pos = 0;
        collect_commands_for_tick(m_current_tick);
        m_state = PlayerState::Paused;
    }

    void ReplayPlayer::seek_to_fraction(float t01) { seek_to_tick(static_cast<uint64_t>(t01 * m_total_ticks)); }

    bool ReplayPlayer::step_forward() {
        m_state = PlayerState::Paused;
        if (m_current_tick >= m_total_ticks) return false;
        if (m_buffer_pos >= m_buffer.size()) {
            while (m_file.good()) {
                ReplayCommand cmd;
                if (!read_next_command(cmd)) break;
                m_buffer.push_back(std::move(cmd));
            }
        }
        collect_commands_for_tick(m_current_tick);
        ++m_current_tick;
        return true;
    }

    bool ReplayPlayer::step_backward() {
        m_state = PlayerState::Paused;
        if (m_current_tick == 0) return false;
        seek_to_tick(m_current_tick - 1);
        return true;
    }

    void ReplayPlayer::update(double real_delta) {
        if (m_state != PlayerState::Playing) return;
        const double ticks_per_sec = 60.0 * m_playback_speed;
        const double advance = real_delta * ticks_per_sec;
        const uint64_t ticks_to_advance = static_cast<uint64_t>(advance);
        for (uint64_t i = 0; i < ticks_to_advance; ++i) {
            if (!step_forward()) {
                m_state = PlayerState::Finished;
                break;
            }
        }
    }

} // namespace Shape::Replay
