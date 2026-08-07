#include "ECS/World.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Biology/BiologySystem.hpp"
#include "AI/OptimizationSystem.hpp"
#include "ECS/CommandBuffer.hpp"

namespace Shape {

World::World() 
    : m_PhysicsSystem(std::make_unique<PhysicsSystem>())
    , m_BiologySystem(std::make_unique<BiologySystem>())
    , m_OptimizationSystem(std::make_unique<OptimizationSystem>())
    , m_CommandBuffer(std::make_unique<CommandBuffer>(*this))
{
}

World::~World() = default;

void World::Tick(f32 dt) {
    m_TickCount++;
    
    // Phase 2: Logic & Update
    m_OptimizationSystem->Update(*this, m_PhysicsSystem->GetGrid(), dt);
    m_BiologySystem->Update(*this, *m_CommandBuffer, dt, m_TickCount);
    m_PhysicsSystem->Update(*this, dt);
    
    // Phase 3: Mutations & Command Buffer Flush
    m_CommandBuffer->Flush();
}

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

void World::SerializeEntities(std::ostream& os) const {
    auto writeVec = [&os](const auto& vec) {
        u32 size = static_cast<u32>(vec.size());
        os.write(reinterpret_cast<const char*>(&size), sizeof(size));
        if (size > 0) {
            os.write(reinterpret_cast<const char*>(vec.data()), size * sizeof(typename std::decay_t<decltype(vec)>::value_type));
        }
    };
    writeVec(m_ActiveEntities);
    writeVec(m_FreeIndices);
    writeVec(m_Generations);
}

void World::DeserializeEntities(std::istream& is) {
    auto readVec = [&is](auto& vec) {
        u32 size = 0;
        is.read(reinterpret_cast<char*>(&size), sizeof(size));
        vec.resize(size);
        if (size > 0) {
            is.read(reinterpret_cast<char*>(vec.data()), size * sizeof(typename std::decay_t<decltype(vec)>::value_type));
        }
    };
    readVec(m_ActiveEntities);
    readVec(m_FreeIndices);
    readVec(m_Generations);
}

} // namespace Shape
