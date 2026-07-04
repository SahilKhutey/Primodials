#include "ECS/World.hpp"

namespace Shape {

Entity World::CreateEntity() {
    u32 index = 0;
    u32 generation = 0;

    if (!m_FreeIndices.empty()) {
        index = m_FreeIndices.back();
        m_FreeIndices.pop_back();
        generation = m_Generations[index];
    } else {
        index = static_cast<u32>(m_Generations.size());
        m_Generations.push_back(0);
    }

    Entity entity = MakeEntity(index, generation);
    m_ActiveEntities.push_back(entity);
    return entity;
}

void World::DestroyEntity(Entity entity) {
    if (!IsEntityValid(entity)) return;

    u32 index = GetEntityIndex(entity);
    
    // Invalidate handle by incrementing generation
    m_Generations[index] = (m_Generations[index] + 1) & ENTITY_GENERATION_MASK;
    m_FreeIndices.push_back(index);

    // Swap-and-pop from active list
    auto it = std::find(m_ActiveEntities.begin(), m_ActiveEntities.end(), entity);
    if (it != m_ActiveEntities.end()) {
        std::swap(*it, m_ActiveEntities.back());
        m_ActiveEntities.pop_back();
    }

    // Purge components of this entity across all registers
    for (auto& [type, store] : m_ComponentStores) {
        store->EntityDestroyed(entity);
    }
}

bool World::IsEntityValid(Entity entity) const {
    if (entity == INVALID_ENTITY) return false;
    
    u32 index = GetEntityIndex(entity);
    return index < m_Generations.size() && m_Generations[index] == GetEntityGeneration(entity);
}

} // namespace Shape
