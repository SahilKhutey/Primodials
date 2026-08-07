#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    std::cout << "[ECS Iteration Benchmark]\n";
    const std::vector<usize> entityCounts = { 1000, 10000, 100000, 1000000 };

    for (usize count : entityCounts) {
        Shape::World world;

        for (usize i = 0; i < count; ++i) {
            Shape::Entity e = world.CreateEntity();
            world.AddComponent<Shape::TransformComponent>(e, Shape::TransformComponent{});
            world.AddComponent<Shape::GeometryComponent>(e, Shape::GeometryComponent(3, 2.0f));
        }

        auto start = std::chrono::high_resolution_clock::now();
        const int iterations = 100;
        for (int iter = 0; iter < iterations; ++iter) {
            auto entities = world.Query<Shape::TransformComponent, Shape::GeometryComponent>();
            for (Shape::Entity e : entities) {
                auto& tf = world.GetComponent<Shape::TransformComponent>(e);
                auto& geom = world.GetComponent<Shape::GeometryComponent>(e);
                tf.position.x += geom.scale * 0.016f;
                tf.position.y += geom.scale * 0.016f;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double avgMs = duration.count() / static_cast<double>(iterations);
        double estFps = avgMs > 0.0001 ? (1000.0 / avgMs) : 9999.0;

        std::cout << "Entities: " << count 
                  << " | Iteration: " << avgMs << " ms"
                  << " | Est. Max FPS: " << estFps << "\n";
    }

    return 0;
}
