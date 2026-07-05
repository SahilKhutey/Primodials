#include "Shape/Network/ENetBackend.h"
#include <memory>
namespace Shape::Network {
    void register_backend(const std::string&, BackendFactory) {}
    std::unique_ptr<INetworkBackend> create_backend(const std::string&) {
        return std::make_unique<ENetBackend>();
    }
}
