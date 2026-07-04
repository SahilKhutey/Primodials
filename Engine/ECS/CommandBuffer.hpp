#pragma once

#include "ECS/World.hpp"
#include <functional>
#include <vector>
#include <mutex>

namespace Shape {

class CommandBuffer {
public:
    explicit CommandBuffer(World& world) : m_world(world) {}
    ~CommandBuffer() = default;

    // Type-safe deferred component addition
    template <typename T>
    void AddComponentDeferred(Entity entity, const T& component) {
        PushCommand([this, entity, component]() {
            m_world.AddComponent<T>(entity, component);
        });
    }

    // Type-safe deferred component removal
    template <typename T>
    void RemoveComponentDeferred(Entity entity) {
        PushCommand([this, entity]() {
            m_world.RemoveComponent<T>(entity);
        });
    }

    // Defer an entity destruction
    void DestroyEntityDeferred(Entity entity) {
        PushCommand([this, entity]() {
            m_world.DestroyEntity(entity);
        });
    }

    // Defer a raw structural mutation safely
    void PushCommand(std::function<void()> command);

    // Flushes all pending mutations
    void Flush();

private:
    World& m_world;
    std::vector<std::function<void()>> m_commands;
    std::mutex m_mutex;
};

} // namespace Shape
