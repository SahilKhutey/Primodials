#pragma once

#include "ECS/Entity.hpp"
#include "Core/Assert.hpp"
#include <vector>
#include <memory>
#include <iostream>

namespace Shape {

class IComponentStore {
public:
    virtual ~IComponentStore() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
    virtual bool Has(Entity entity) const = 0;
    virtual void Remove(Entity entity) = 0;
    virtual void Serialize(std::ostream& os) const = 0;
    virtual void Deserialize(std::istream& is) = 0;
    virtual u32 GetComponentTypeId() const = 0;
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

    u32 GetComponentTypeId() const override {
        return T::TypeId;
    }

    void Serialize(std::ostream& os) const override {
        // Write size
        u32 count = static_cast<u32>(m_DenseComponents.size());
        os.write(reinterpret_cast<const char*>(&count), sizeof(count));

        if (count > 0) {
            // Write dense components array
            os.write(reinterpret_cast<const char*>(m_DenseComponents.data()), count * sizeof(T));
            // Write dense entities array
            os.write(reinterpret_cast<const char*>(m_DenseEntities.data()), count * sizeof(Entity));
            
            // Write sparse array
            u32 sparseSize = static_cast<u32>(m_Sparse.size());
            os.write(reinterpret_cast<const char*>(&sparseSize), sizeof(sparseSize));
            os.write(reinterpret_cast<const char*>(m_Sparse.data()), sparseSize * sizeof(u32));
        }
    }

    void Deserialize(std::istream& is) override {
        u32 count = 0;
        is.read(reinterpret_cast<char*>(&count), sizeof(count));

        m_DenseComponents.resize(count);
        m_DenseEntities.resize(count);

        if (count > 0) {
            is.read(reinterpret_cast<char*>(m_DenseComponents.data()), count * sizeof(T));
            is.read(reinterpret_cast<char*>(m_DenseEntities.data()), count * sizeof(Entity));

            u32 sparseSize = 0;
            is.read(reinterpret_cast<char*>(&sparseSize), sizeof(sparseSize));
            m_Sparse.resize(sparseSize);
            is.read(reinterpret_cast<char*>(m_Sparse.data()), sparseSize * sizeof(u32));
        } else {
            m_Sparse.clear();
        }
    }

private:
    // Dense contiguous arrays
    std::vector<T> m_DenseComponents;
    std::vector<Entity> m_DenseEntities;

    // Sparse mapping array: entity index -> dense index
    std::vector<u32> m_Sparse;
};

} // namespace Shape
