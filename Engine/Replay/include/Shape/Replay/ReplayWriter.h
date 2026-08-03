// Shape/Replay/ReplayWriter.h
#pragma once

#include "Shape/Replay/ReplayFormat.h"
#include "Shape/Simulation/SimulationState.hpp" // Use SimulationClock or primitive
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>

namespace Shape::Replay {

    class ReplayWriter {
    public:
        ReplayWriter();
        ~ReplayWriter();

        bool begin(const std::string& path, uint64_t seed, uint64_t start_time);
        void end();

        bool write_command(uint64_t tick, CommandType type, const void* data, uint32_t size);
        bool write_command(uint64_t tick, CommandType type, const std::vector<uint8_t>& data);

        bool write_world_state(const void* data, uint32_t size);

        bool is_open() const { return m_file.is_open(); }
        uint64_t bytes_written() const { return m_bytes_written; }
        uint64_t command_count() const { return m_command_count; }

    private:
        std::ofstream m_file;
        uint64_t m_bytes_written = 0;
        uint64_t m_command_count = 0;
        uint32_t m_checksum = 0;
    };

} // namespace Shape::Replay
