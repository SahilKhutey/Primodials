#pragma once

#include "Core/Platform.hpp"
#include <vector>
#include <string>
#include <functional>
#include <optional>

namespace Shape::Network {

struct Endpoint {
    std::string address = "127.0.0.1";
    u16 port = 7777;
    u64 id = 0;

    bool IsValid() const { return id != 0; }
};

struct Packet {
    std::vector<u8> data;
    Endpoint sender;
    bool isReliable = true;
};

class ITransport {
public:
    using PacketCallback = std::function<void(const Packet& packet)>;
    using ConnectCallback = std::function<void(Endpoint peer)>;
    using DisconnectCallback = std::function<void(Endpoint peer, const std::string& reason)>;

    virtual ~ITransport() = default;

    virtual bool Initialize(u16 port = 0, i32 maxClients = 16) = 0;
    virtual void Shutdown() = 0;
    virtual void Poll() = 0;

    virtual bool Listen(u16 port) = 0;
    virtual std::optional<Endpoint> Accept() = 0;
    virtual void Disconnect(Endpoint peer, const std::string& reason = "") = 0;

    virtual bool Connect(const std::string& host, u16 port, u32 timeoutMs = 5000) = 0;
    virtual bool Send(Endpoint peer, const void* data, usize size, bool reliable = true) = 0;
    virtual bool Broadcast(const void* data, usize size, bool reliable = true) = 0;
    virtual std::optional<Packet> Receive() = 0;

    virtual u32 GetRTT(Endpoint peer) const = 0;
    virtual f32 GetPacketLoss(Endpoint peer) const = 0;
    virtual u64 GetBytesSent() const = 0;
    virtual u64 GetBytesReceived() const = 0;

    void SetPacketCallback(PacketCallback cb) { m_packetCb = std::move(cb); }
    void SetConnectCallback(ConnectCallback cb) { m_connectCb = std::move(cb); }
    void SetDisconnectCallback(DisconnectCallback cb) { m_disconnectCb = std::move(cb); }

protected:
    PacketCallback m_packetCb;
    ConnectCallback m_connectCb;
    DisconnectCallback m_disconnectCb;
};

} // namespace Shape::Network
