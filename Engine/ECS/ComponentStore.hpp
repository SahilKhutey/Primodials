#pragma once

#include "ECS/Entity.hpp"
#include "Core/Assert.hpp"
#include <vector>
#include <memory>

namespace Shape {

class IComponentStore {
public:
    virtual ~IComponentStore() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
    virtual bool Has(Entity entity) const = 0;
    virtual void Remove(Entity entity) = 0;
};

template <typename T>
class ComponentStore : public IComponentStore {
public:
    ComponentStore() = default;
    ~ComponentStore() override = default;

    void Add(Entity entity, const T& component) {
        u32 index = GetEntityIndex(entity);
        if (index >= m_Sparse.size()) {
            m_Sparse.resize(index + 1, 0xFFFFFFFF);
        }

        SHAPE_ASSERT_MSG(m_Sparse[index] == 0xFFFFFFFF, "Entity {} already has component!", entity);

        m_Sparse[index] = static_cast<u32>(m_DenseComponents.size());
        m_DenseComponents.push_back(component);
        m_DenseEntities.push_back(entity);
    }

    void Remove(Entity entity) override {
        if (!Has(entity)) return;

        u32 index = GetEntityIndex(entity);
        u32 denseIdx = m_Sparse[index];
        u32 lastIdx = static_cast<u32>(m_DenseComponents.size()) - 1;

        if (denseIdx < lastIdx) {
            // Swap last element with the removed element
            m_DenseComponents[denseIdx] = std::move(m_DenseComponents[lastIdx]);
            
            Entity swappedEntity = m_DenseEntities[lastIdx];
            m_DenseEntities[denseIdx] = swappedEntity;
            
            m_Sparse[GetEntityIndex(swappedEntity)] = denseIdx;
        }

        m_DenseComponents.pop_back();
        m_DenseEntities.pop_back();
        m_Sparse[index] = 0xFFFFFFFF;
    }

    bool Has(Entity entity) const override {
        u32 index = GetEntityIndex(entity);
        return index < m_Sparse.size() && m_Sparse[index] != 0xFFFFFFFF;
    }

    T& Get(Entity entity) {
        SHAPE_ASSERT_MSG(Has(entity), "Entity {} does not have component", entity);
        return m_DenseComponents[m_Sparse[GetEntityIndex(entity)]];
    }

    const T& Get(Entity entity) const {
        SHAPE_ASSERT_MSG(Has(entity), "Entity {} does not have component", entity);
        return m_DenseComponents[m_Sparse[GetEntityIndex(entity)]];
    }

    void EntityDestroyed(Entity entity) override {
        if (Has(entity)) {
            Remove(entity);
        }
    }

    // Accessors for dense arrays to enable high performance iterations
    T* GetData() { return m_DenseComponents.data(); }
    const T* GetData() const { return m_DenseComponents.data(); }
    
    Entity* GetEntities() { return m_DenseEntities.data(); }
    const Entity* GetEntities() const { return m_DenseEntities.data(); }
    
    usize GetSize() const { return m_DenseComponents.size(); }

private:
    // Dense contiguous arrays
    std::vector<T> m_DenseComponents;
    std::vector<Entity> m_DenseEntities;

    // Sparse mapping array: entity index -> dense index
    std::vector<u32> m_Sparse;
};

} // namespace Shape
