// Shape/Replay/ReplayWriter.cpp
#include "Shape/Replay/ReplayWriter.h"
#include "Shape/Core/Logger.hpp"

#include <iostream>

namespace Shape::Replay {

    ReplayWriter::ReplayWriter() = default;
    ReplayWriter::~ReplayWriter() { end(); }

    bool ReplayWriter::begin(const std::string& path, uint64_t seed, uint64_t start_time) {
        m_file.open(path, std::ios::binary | std::ios::out | std::ios::trunc);
        if (!m_file) {
            SHAPE_LOG_ERROR("Replay", "Failed to open: {}", path.c_str());
            return false;
        }
        ReplayHeader header;
        header.magic = REPLAY_MAGIC;
        header.version = REPLAY_VERSION;
        header.seed = seed;
        header.start_time = start_time;
        header.world_size_bytes = 0;
        header.expected_ticks = 0;
        header.reserved = 0;
        m_file.write(reinterpret_cast<const char*>(&header), sizeof(header));
        m_bytes_written = sizeof(header);
        m_command_count = 0;
        m_checksum = 0;
        return true;
    }

    void ReplayWriter::end() {
        if (!m_file.is_open()) return;
        FileFooter footer;
        footer.total_ticks = 0;  // Updated by caller
        footer.total_commands = m_command_count;
        footer.checksum = m_checksum;
        footer.reserved = 0;
        m_file.write(reinterpret_cast<const char*>(&footer), sizeof(footer));
        m_bytes_written += sizeof(footer);
        m_file.close();
    }

    bool ReplayWriter::write_command(uint64_t tick, CommandType type, const void* data, uint32_t size) {
        if (!m_file.is_open()) return false;
        CommandHeader ch;
        ch.tick = tick;
        ch.type = type;
        ch.size = size;
        m_file.write(reinterpret_cast<const char*>(&ch), sizeof(ch));
        if (size > 0 && data) {
            m_file.write(reinterpret_cast<const char*>(data), size);
        }
        m_bytes_written += sizeof(ch) + size;
        ++m_command_count;

        // Skip checksum for now
        return m_file.good();
    }

    bool ReplayWriter::write_command(uint64_t tick, CommandType type, const std::vector<uint8_t>& data) {
        return write_command(tick, type, data.data(), static_cast<uint32_t>(data.size()));
    }

    bool ReplayWriter::write_world_state(const void* data, uint32_t size) {
        if (!m_file.is_open()) return false;
        m_file.write(reinterpret_cast<const char*>(data), size);
        m_bytes_written += size;
        return m_file.good();
    }

} // namespace Shape::Replay
