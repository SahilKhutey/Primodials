#pragma once
#include "Shape/Network/INetworkBackend.h"
namespace Shape::Network {
    class ENetBackend : public INetworkBackend {
    public:
        bool initialize() override { return true; }
        void shutdown() override {}
        void poll() override {}
        bool start_server(uint16_t, int) override { return true; }
        void stop_server() override {}
        bool connect(const std::string&, uint16_t) override { return true; }
        void disconnect() override {}
        void send(uint32_t, const Packet&) override {}
        void broadcast(const Packet&) override {}
        bool receive(uint32_t&, Packet&) override { return false; }
        PeerInfo get_peer_info(uint32_t) const override { return {}; }
        int connected_count() const override { return 0; }
        bool is_server() const override { return false; }
        bool is_connected() const override { return false; }
    };
}
