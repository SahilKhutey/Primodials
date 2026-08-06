#include "Spatial/BroadPhaseSystem.hpp"

namespace Shape::Spatial {

BroadPhaseSystem::BroadPhaseSystem(f32 cellSize)
    : m_spatialHash(cellSize) {}

void BroadPhaseSystem::Update(World& world, const UpdateContext& ctx) {
    (void)ctx;
    m_spatialHash.Clear();

    auto entities = world.Query<TransformComponent>();
    for (Entity e : entities) {
        const auto& tf = world.GetComponent<TransformComponent>(e);
        m_spatialHash.Insert(e, tf.position);
    }
}

} // namespace Shape::Spatial
