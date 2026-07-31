// Shape/Simulation/Systems/GroupBehaviorSystem.cpp
#include "Shape/Simulation/Systems/GroupBehaviorSystem.h"
#include <cmath>

namespace Shape::Simulation {

GroupBehaviorSystem::GroupBehaviorSystem(World* world)
    : m_world(world) {}

void GroupBehaviorSystem::update(const TickContext& ctx) {
    (void)ctx;
    if (!m_world) return;

    auto entities = m_world->Query<FlockComponent, TransformComponent, RigidBodyComponent>();
    if (entities.size() < 2) return;

    // Cache components for iteration
    std::vector<std::pair<Entity, FlockComponent>> flock_entities;
    flock_entities.reserve(entities.size());
    for (Entity e : entities) {
        flock_entities.emplace_back(e, m_world->GetComponent<FlockComponent>(e));
    }

    for (size_t i = 0; i < flock_entities.size(); ++i) {
        Entity entity = flock_entities[i].first;
        const auto& flock = flock_entities[i].second;
        const auto& transform = m_world->GetComponent<TransformComponent>(entity);
        auto& rb = m_world->GetComponent<RigidBodyComponent>(entity);

        // Find neighbors in same flock within perception radius
        std::vector<std::pair<TransformComponent, RigidBodyComponent>> neighbors;
        for (size_t j = 0; j < flock_entities.size(); ++j) {
            if (i == j) continue;
            if (flock_entities[j].second.flock_id != flock.flock_id) continue;

            Entity other_e = flock_entities[j].first;
            const auto& other_t = m_world->GetComponent<TransformComponent>(other_e);
            const auto& other_rb = m_world->GetComponent<RigidBodyComponent>(other_e);

            f32 dist_sq = (transform.position - other_t.position).LengthSquared();
            if (dist_sq <= flock.perception_radius * flock.perception_radius && dist_sq > 0.0001f) {
                neighbors.emplace_back(other_t, other_rb);
            }
        }

        if (neighbors.empty()) continue;

        Math::Vector2f sep = compute_separation(transform, neighbors, flock.perception_radius) * flock.separation_weight;
        Math::Vector2f ali = compute_alignment(rb, neighbors) * flock.alignment_weight;
        Math::Vector2f coh = compute_cohesion(transform, neighbors) * flock.cohesion_weight;

        Math::Vector2f steering_force = sep + ali + coh;

        // Clamp steering force to max_force
        f32 force_len_sq = steering_force.LengthSquared();
        if (force_len_sq > flock.max_force * flock.max_force && force_len_sq > 0.0f) {
            f32 force_len = std::sqrt(force_len_sq);
            steering_force = (steering_force / force_len) * flock.max_force;
        }

        rb.force += steering_force;
    }
}

Math::Vector2f GroupBehaviorSystem::compute_separation(const TransformComponent& transform,
                                                        const std::vector<std::pair<TransformComponent, RigidBodyComponent>>& neighbors,
                                                        f32 perception_radius) {
    (void)perception_radius;
    Math::Vector2f steering{0.0f, 0.0f};
    size_t count = 0;

    for (const auto& neighbor : neighbors) {
        Math::Vector2f diff = transform.position - neighbor.first.position;
        f32 dist_sq = diff.LengthSquared();
        if (dist_sq > 0.0001f) {
            f32 dist = std::sqrt(dist_sq);
            steering += (diff / dist) / dist; // Weight inversely by distance
            count++;
        }
    }

    if (count > 0) {
        steering = steering / static_cast<f32>(count);
    }
    return steering;
}

Math::Vector2f GroupBehaviorSystem::compute_alignment(const RigidBodyComponent& rb,
                                                       const std::vector<std::pair<TransformComponent, RigidBodyComponent>>& neighbors) {
    (void)rb;
    Math::Vector2f avg_velocity{0.0f, 0.0f};
    if (neighbors.empty()) return avg_velocity;

    for (const auto& neighbor : neighbors) {
        avg_velocity += neighbor.second.velocity;
    }

    avg_velocity = avg_velocity / static_cast<f32>(neighbors.size());
    return avg_velocity;
}

Math::Vector2f GroupBehaviorSystem::compute_cohesion(const TransformComponent& transform,
                                                      const std::vector<std::pair<TransformComponent, RigidBodyComponent>>& neighbors) {
    Math::Vector2f center_of_mass{0.0f, 0.0f};
    if (neighbors.empty()) return center_of_mass;

    for (const auto& neighbor : neighbors) {
        center_of_mass += neighbor.first.position;
    }

    center_of_mass = center_of_mass / static_cast<f32>(neighbors.size());
    return center_of_mass - transform.position; // Steering towards center of mass
}

} // namespace Shape::Simulation
