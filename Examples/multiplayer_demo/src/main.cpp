#include <iostream>
#include "Shape/Network/LockstepSession.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::cout << "Starting Multiplayer Demo...\n";
    Shape::Network::LockstepSession session;
    Shape::Network::LockstepConfig cfg;
    session.host(cfg);
    std::cout << "Host started on port " << cfg.port << "\n";
    return 0;
}
