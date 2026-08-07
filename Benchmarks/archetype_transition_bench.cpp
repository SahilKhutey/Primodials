#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    std::cout << "[Archetype Transition Benchmark]\n";
    const usize entityCount = 50000;
    Shape::World world;
    std::vector<Shape::Entity> entities;
    entities.reserve(entityCount);

    for (usize i = 0; i < entityCount; ++i) {
        Shape::Entity e = world.CreateEntity();
        world.AddComponent<Shape::TransformComponent>(e, Shape::TransformComponent{});
        entities.push_back(e);
    }

    auto start = std::chrono::high_resolution_clock::now();
    
    for (Shape::Entity e : entities) {
        world.AddComponent<Shape::GeometryComponent>(e, Shape::GeometryComponent(3, 2.0f));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Archetype transitions for " << entityCount << " entities: " 
              << duration.count() << " ms (" 
              << (duration.count() / entityCount * 1000.0) << " us/entity)\n";

    return 0;
}
