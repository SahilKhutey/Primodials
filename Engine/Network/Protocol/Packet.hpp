#pragma once

#include "Core/Platform.hpp"
#include "Serialization/SimSerializer.hpp"
#include <vector>
#include <string>
#include <cstring>

namespace Shape::Network {

enum class PacketType : u8 {
    Hello           = 0x01,
    Welcome         = 0x02,
    Ping            = 0x03,
    Pong            = 0x04,
    Disconnect      = 0x05,
    Ready           = 0x10,
    StartGame       = 0x11,
    InputBatch      = 0x20,
    StateHash       = 0x21,
    ChatMessage     = 0x30
};

class PacketBuilder {
public:
    explicit PacketBuilder(PacketType type) {
        m_buffer.push_back(static_cast<u8>(type));
        m_buffer.push_back(0); // Flags
        u32 dummyLength = 0;
        usize lenPos = m_buffer.size();
        m_buffer.resize(lenPos + sizeof(u32));
        std::memcpy(m_buffer.data() + lenPos, &dummyLength, sizeof(u32));
    }

    void WriteU8(u8 val) { m_buffer.push_back(val); }
    void WriteU32(u32 val) {
        usize pos = m_buffer.size();
        m_buffer.resize(pos + sizeof(u32));
        std::memcpy(m_buffer.data() + pos, &val, sizeof(u32));
    }
    void WriteU64(u64 val) {
        usize pos = m_buffer.size();
        m_buffer.resize(pos + sizeof(u64));
        std::memcpy(m_buffer.data() + pos, &val, sizeof(u64));
    }
    void WriteF32(f32 val) {
        usize pos = m_buffer.size();
        m_buffer.resize(pos + sizeof(f32));
        std::memcpy(m_buffer.data() + pos, &val, sizeof(f32));
    }
    void WriteString(const std::string& str) {
        WriteU32(static_cast<u32>(str.size()));
        usize pos = m_buffer.size();
        m_buffer.resize(pos + str.size());
        std::memcpy(m_buffer.data() + pos, str.data(), str.size());
    }

    std::vector<u8> Build() {
        u32 payloadLen = static_cast<u32>(m_buffer.size() - 6);
        std::memcpy(m_buffer.data() + 2, &payloadLen, sizeof(u32));
        return m_buffer;
    }

private:
    std::vector<u8> m_buffer;
};

class PacketReader {
public:
    explicit PacketReader(const u8* data, usize size)
        : m_data(data), m_size(size), m_offset(0) {
        if (m_size >= 6) {
            m_type = static_cast<PacketType>(m_data[0]);
            m_flags = m_data[1];
            std::memcpy(&m_payloadLength, m_data + 2, sizeof(u32));
            m_offset = 6;
        }
    }

    PacketType GetType() const { return m_type; }
    u32 GetPayloadLength() const { return m_payloadLength; }

    u8 ReadU8() {
        if (m_offset >= m_size) return 0;
        return m_data[m_offset++];
    }
    u32 ReadU32() {
        if (m_offset + sizeof(u32) > m_size) return 0;
        u32 val = 0;
        std::memcpy(&val, m_data + m_offset, sizeof(u32));
        m_offset += sizeof(u32);
        return val;
    }
    u64 ReadU64() {
        if (m_offset + sizeof(u64) > m_size) return 0;
        u64 val = 0;
        std::memcpy(&val, m_data + m_offset, sizeof(u64));
        m_offset += sizeof(u64);
        return val;
    }
    f32 ReadF32() {
        if (m_offset + sizeof(f32) > m_size) return 0.0f;
        f32 val = 0.0f;
        std::memcpy(&val, m_data + m_offset, sizeof(f32));
        m_offset += sizeof(f32);
        return val;
    }
    std::string ReadString() {
        u32 len = ReadU32();
        if (m_offset + len > m_size) return "";
        std::string str(reinterpret_cast<const char*>(m_data + m_offset), len);
        m_offset += len;
        return str;
    }

private:
    const u8* m_data = nullptr;
    usize m_size = 0;
    usize m_offset = 0;
    PacketType m_type = PacketType::Hello;
    u8 m_flags = 0;
    u32 m_payloadLength = 0;
};

} // namespace Shape::Network
