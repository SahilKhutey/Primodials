#define private public
#define protected public
#include <iostream>
#include "ECS2/World2.hpp"
#include "ECS2/ComponentRegistry.hpp"
#include <cstddef>

using namespace Shape::ECS2;

struct Dummy1 { int x; };
struct Dummy2 { int x; };
struct Pos2D { float x = 0, y = 0; };
struct Vel2D { float vx = 0, vy = 0; };

struct EcsRegistrar {
    EcsRegistrar() {
        ComponentRegistry::register_type<Dummy1>("Dummy1");
        ComponentRegistry::register_type<Dummy2>("Dummy2");
        ComponentRegistry::register_type<Vel2D>("Vel2D");
        ComponentRegistry::register_type<Pos2D>("Pos2D");
    }
};
static EcsRegistrar _reg;

int main() {
    World2 world;
    EntityId e = world.create();
    world.add<Pos2D>(e, {1, 2});
    
    std::cout << "Pos2D x before: " << world.get<Pos2D>(e).x << "\n";
    
    // NO get_or_create_storage!
    world.add<Vel2D>(e, {5, 6});
    
    std::cout << "Pos2D x after: " << world.get<Pos2D>(e).x << "\n";
    
    Pos2D* pDst = &world.get<Pos2D>(e);
    Vel2D* vDst = &world.get<Vel2D>(e);
    std::cout << "Pos2D in new chunk: " << (void*)pDst << "\n";
    std::cout << "Vel2D in new chunk: " << (void*)vDst << "\n";
    std::cout << "Pos2D - Vel2D diff: " << (ptrdiff_t)((u8*)pDst - (u8*)vDst) << "\n";
    
    return 0;
}
