#pragma once

#include "Network/Transport/ITransport.hpp"
#include <queue>
#include <mutex>

namespace Shape::Network {

class LoopbackTransport : public ITransport {
public:
    LoopbackTransport() = default;
    ~LoopbackTransport() override = default;

    bool Initialize(u16 port = 0, i32 maxClients = 16) override {
        (void)maxClients;
        m_localEndpoint.id = 1;
        m_localEndpoint.port = port;
        m_initialized = true;
        return true;
    }

    void Shutdown() override {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::queue<Packet> empty;
        std::swap(m_packets, empty);
        m_initialized = false;
    }

    void Poll() override {}

    bool Listen(u16 port) override {
        m_localEndpoint.port = port;
        return true;
    }

    std::optional<Endpoint> Accept() override { return std::nullopt; }
    void Disconnect(Endpoint peer, const std::string& reason = "") override {
        (void)peer; (void)reason;
    }

    bool Connect(const std::string& host, u16 port, u32 timeoutMs = 5000) override {
        (void)host; (void)port; (void)timeoutMs;
        return true;
    }

    bool Send(Endpoint peer, const void* data, usize size, bool reliable = true) override {
        if (!m_initialized) return false;
        Packet p;
        p.data.assign(reinterpret_cast<const u8*>(data), reinterpret_cast<const u8*>(data) + size);
        p.sender = peer;
        p.isReliable = reliable;

        std::lock_guard<std::mutex> lock(m_mutex);
        m_packets.push(p);
        m_bytesSent += size;
        return true;
    }

    bool Broadcast(const void* data, usize size, bool reliable = true) override {
        Endpoint ep;
        ep.id = 999;
        return Send(ep, data, size, reliable);
    }

    std::optional<Packet> Receive() override {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_packets.empty()) return std::nullopt;
        Packet p = m_packets.front();
        m_packets.pop();
        m_bytesReceived += p.data.size();
        return p;
    }

    u32 GetRTT(Endpoint peer) const override { (void)peer; return 1; }
    f32 GetPacketLoss(Endpoint peer) const override { (void)peer; return 0.0f; }
    u64 GetBytesSent() const override { return m_bytesSent; }
    u64 GetBytesReceived() const override { return m_bytesReceived; }

private:
    Endpoint m_localEndpoint;
    std::queue<Packet> m_packets;
    std::mutex m_mutex;
    u64 m_bytesSent = 0;
    u64 m_bytesReceived = 0;
    bool m_initialized = false;
};

} // namespace Shape::Network
