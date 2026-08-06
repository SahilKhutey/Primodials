#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include "Shape/Simulation/SimulationController.h"
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    std::cout << "[Full Simulation Tick Benchmark]\n";
    const std::vector<usize> scales = { 1000, 10000, 50000 };

    for (usize scale : scales) {
        Shape::World world;
        Shape::Simulation::SimulationController controller;

        for (usize i = 0; i < scale; ++i) {
            Shape::Entity e = world.CreateEntity();
            world.AddComponent<Shape::TransformComponent>(e, Shape::TransformComponent{});
            world.AddComponent<Shape::GeometryComponent>(e, Shape::GeometryComponent(4, 3.0f));
        }

        const int ticks = 60;
        auto start = std::chrono::high_resolution_clock::now();
        for (int t = 0; t < ticks; ++t) {
            controller.Update(16666); // 16.6 ms tick
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double avgTickMs = duration.count() / ticks;
        std::cout << "Entities: " << scale 
                  << " | 60-Tick Total: " << duration.count() << " ms"
                  << " | Avg Tick: " << avgTickMs << " ms/tick"
                  << " | Sim FPS: " << (avgTickMs > 0.0001 ? (1000.0 / avgTickMs) : 9999.0) << "\n";
    }

    return 0;
}
