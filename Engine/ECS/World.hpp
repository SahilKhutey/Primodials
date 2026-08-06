#pragma once

#include "ECS/Entity.hpp"
#include "ECS/ComponentStore.hpp"
#include "Core/Assert.hpp"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

namespace Shape {

class PhysicsSystem;
class BiologySystem;
class OptimizationSystem;
class CommandBuffer;

class World {
public:
    World();
    ~World();

    void Tick(f32 dt);

    // Entity Management
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    bool IsEntityValid(Entity entity) const;

    // Serialization
    void SerializeEntities(std::ostream& os) const;
    void DeserializeEntities(std::istream& is);

    // Component Management
    template <typename T>
    void AddComponent(Entity entity, const T& component) {
        SHAPE_ASSERT_MSG(IsEntityValid(entity), "Attempted to add component to invalid entity {}", entity);
        GetComponentStore<T>().Add(entity, component);
    }

    template <typename T>
    void RemoveComponent(Entity entity) {
        if (!IsEntityValid(entity)) return;
        GetComponentStore<T>().Remove(entity);
    }

    template <typename T>
    T& GetComponent(Entity entity) {
        SHAPE_ASSERT_MSG(IsEntityValid(entity), "Attempted to get component from invalid entity {}", entity);
        return GetComponentStore<T>().Get(entity);
    }

    template <typename T>
    const T& GetComponent(Entity entity) const {
        SHAPE_ASSERT_MSG(IsEntityValid(entity), "Attempted to get component from invalid entity {}", entity);
        return GetComponentStore<T>().Get(entity);
    }

    template <typename T>
    const ComponentStore<T>* FindComponentStore() const {
        u32 type_id = T::TypeId;
        auto it = m_StoresByTypeId.find(type_id);
        if (it != m_StoresByTypeId.end()) {
            return static_cast<const ComponentStore<T>*>(it->second);
        }
        auto typeIdx = std::type_index(typeid(T));
        auto it2 = m_ComponentStores.find(typeIdx);
        if (it2 != m_ComponentStores.end()) {
            return static_cast<const ComponentStore<T>*>(it2->second.get());
        }
        return nullptr;
    }

    template <typename T>
    bool HasComponent(Entity entity) const {
        if (!IsEntityValid(entity)) return false;
        const auto* store = FindComponentStore<T>();
        if (!store) return false;
        return store->Has(entity);
    }

    // Queries: Returns a list of entities containing all specified components
    template <typename... Components>
    std::vector<Entity> Query() {
        std::vector<Entity> results;
        for (Entity entity : m_ActiveEntities) {
            if ((HasComponent<Components>(entity) && ...)) {
                results.push_back(entity);
            }
        }
        return results;
    }

    // Diagnostics
    usize GetActiveEntityCount() const { return m_ActiveEntities.size(); }
    u32 GetTickCount() const { return m_TickCount; }

    template <typename T>
    ComponentStore<T>& GetComponentStore() {
        u32 type_id = T::TypeId;
        auto it = m_StoresByTypeId.find(type_id);
        if (it == m_StoresByTypeId.end()) {
            auto store = std::make_unique<ComponentStore<T>>();
            auto* rawStore = store.get();
            auto typeIdx = std::type_index(typeid(T));
            m_StoresByTypeId[type_id] = rawStore;
            m_ComponentStores[typeIdx] = std::move(store);
            return *rawStore;
        }
        return *static_cast<ComponentStore<T>*>(it->second);
    }

    template <typename T>
    const ComponentStore<T>& GetComponentStore() const {
        const auto* store = FindComponentStore<T>();
        SHAPE_ASSERT_MSG(store != nullptr, "Component store not found!");
        return *store;
    }

    const std::unordered_map<std::type_index, std::unique_ptr<IComponentStore>>& GetComponentStores() const {
        return m_ComponentStores;
    }
    
    // Allows Serializer to dynamically create a store by type index
    template <typename T>
    void EnsureComponentStore() {
        GetComponentStore<T>(); // Creates if it doesn't exist
    }

private:
    // Entity lists
    std::vector<Entity> m_ActiveEntities;
    std::vector<u32> m_FreeIndices;
    std::vector<u32> m_Generations;

    // Component Registry
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStore>> m_ComponentStores;
    std::unordered_map<u32, IComponentStore*> m_StoresByTypeId;
    
    // Systems
    std::unique_ptr<PhysicsSystem> m_PhysicsSystem;
    std::unique_ptr<BiologySystem> m_BiologySystem;
    std::unique_ptr<OptimizationSystem> m_OptimizationSystem;
    std::unique_ptr<CommandBuffer> m_CommandBuffer;
    
    u32 m_TickCount = 0;
};

} // namespace Shape
