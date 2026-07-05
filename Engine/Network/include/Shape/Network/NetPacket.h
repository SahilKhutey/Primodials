// Shape/Network/NetPacket.h
#pragma once
#include <cstdint>
#include <vector>
#include <span>
#include <cstring>
#include "Shape/Core/Assert.hpp"

namespace Shape::Network {

    enum class PacketType : uint8_t {
        Hello = 1,
        Welcome = 2,
        Heartbeat = 3,
        Input = 10,
        Command = 11,
        StateHash = 20,
        Snapshot = 21,
        Delta = 22,
        DesyncReport = 30,
        Chat = 40,
        Disconnect = 99
    };

    struct PacketHeader {
        uint32_t magic = 0x53484E50;  // 'SHNP'
        uint8_t  version = 1;
        PacketType type = PacketType::Hello;
        uint32_t sequence = 0;
        uint32_t ack = 0;
        uint32_t body_size = 0;
        uint32_t checksum = 0;  // CRC32 of body
    };

    constexpr size_t MAX_PACKET_SIZE = 1400;  // Safe MTU
    constexpr size_t HEADER_SIZE = sizeof(PacketHeader);

    class Packet {
    public:
        Packet() = default;
        explicit Packet(PacketType t) { m_header.type = t; }

        // Header access
        PacketHeader& header() { return m_header; }
        const PacketHeader& header() const { return m_header; }

        // Body access
        std::vector<uint8_t>& body() { return m_body; }
        const std::vector<uint8_t>& body() const { return m_body; }
        void set_body(std::vector<uint8_t> b) { m_body = std::move(b); m_header.body_size = static_cast<uint32_t>(m_body.size()); }

        // Serialization
        std::vector<uint8_t> serialize() const {
            std::vector<uint8_t> data(HEADER_SIZE + m_body.size());
            std::memcpy(data.data(), &m_header, HEADER_SIZE);
            if (!m_body.empty()) {
                std::memcpy(data.data() + HEADER_SIZE, m_body.data(), m_body.size());
            }
            return data;
        }

        static Packet deserialize(const uint8_t* data, size_t size) {
            Packet p;
            if (size >= HEADER_SIZE) {
                std::memcpy(&p.m_header, data, HEADER_SIZE);
                if (p.m_header.body_size > 0 && size >= HEADER_SIZE + p.m_header.body_size) {
                    p.m_body.resize(p.m_header.body_size);
                    std::memcpy(p.m_body.data(), data + HEADER_SIZE, p.m_header.body_size);
                }
            }
            return p;
        }

        bool is_valid() const { return m_header.magic == 0x53484E50; }

        // Body write helpers
        template<typename T>
        void write(const T& value) {
            const size_t offset = m_body.size();
            m_body.resize(offset + sizeof(T));
            std::memcpy(m_body.data() + offset, &value, sizeof(T));
            m_header.body_size = static_cast<uint32_t>(m_body.size());
        }

        template<typename T>
        T read(size_t& offset) const {
            T v;
            SHAPE_ASSERT(offset + sizeof(T) <= m_body.size());
            std::memcpy(&v, m_body.data() + offset, sizeof(T));
            offset += sizeof(T);
            return v;
        }

    private:
        PacketHeader m_header;
        std::vector<uint8_t> m_body;
    };

} // namespace Shape::Network
