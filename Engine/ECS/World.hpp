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

class World {
public:
    World() = default;
    ~World() = default;

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
    bool HasComponent(Entity entity) const {
        if (!IsEntityValid(entity)) return false;
        auto it = m_ComponentStores.find(std::type_index(typeid(T)));
        if (it == m_ComponentStores.end()) return false;
        return it->second->Has(entity);
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

    template <typename T>
    ComponentStore<T>& GetComponentStore() {
        auto typeIdx = std::type_index(typeid(T));
        auto it = m_ComponentStores.find(typeIdx);
        if (it == m_ComponentStores.end()) {
            auto store = std::make_unique<ComponentStore<T>>();
            auto* rawStore = store.get();
            m_ComponentStores[typeIdx] = std::move(store);
            return *rawStore;
        }
        return *static_cast<ComponentStore<T>*>(it->second.get());
    }

    template <typename T>
    const ComponentStore<T>& GetComponentStore() const {
        auto typeIdx = std::type_index(typeid(T));
        auto it = m_ComponentStores.find(typeIdx);
        SHAPE_ASSERT_MSG(it != m_ComponentStores.end(), "Component store not found!");
        return *static_cast<const ComponentStore<T>*>(it->second.get());
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
};

} // namespace Shape
