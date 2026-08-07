// Shape/Network/INetworkBackend.h
#pragma once

#include "Shape/Network/NetPacket.h"
#include <string>
#include <functional>
#include <memory>
#include <cstdint>

namespace Shape::Network {

    enum class ConnectionState : uint8_t {
        Disconnected,
        Connecting,
        Connected,
        Disconnecting
    };

    struct PeerInfo {
        uint32_t id = 0;
        std::string address;
        uint16_t port = 0;
        ConnectionState state = ConnectionState::Disconnected;
        double latency_ms = 0;
        float packet_loss = 0;
    };

    class INetworkBackend {
    public:
        virtual ~INetworkBackend() = default;
        virtual bool initialize() = 0;
        virtual void shutdown() = 0;
        virtual void poll() = 0;

        // Server
        virtual bool start_server(uint16_t port, int max_clients) = 0;
        virtual void stop_server() = 0;

        // Client
        virtual bool connect(const std::string& address, uint16_t port) = 0;
        virtual void disconnect() = 0;

        // Send
        virtual void send(uint32_t peer_id, const Packet& packet) = 0;
        virtual void broadcast(const Packet& packet) = 0;

        // Receive
        virtual bool receive(uint32_t& peer_id, Packet& packet) = 0;

        // State
        virtual PeerInfo get_peer_info(uint32_t peer_id) const = 0;
        virtual int connected_count() const = 0;
        virtual bool is_server() const = 0;
        virtual bool is_connected() const = 0;
    };

    using BackendFactory = std::function<std::unique_ptr<INetworkBackend>()>;
    void register_backend(const std::string& name, BackendFactory factory);
    std::unique_ptr<INetworkBackend> create_backend(const std::string& name);

} // namespace Shape::Network
