#include <catch2/catch_test_macros.hpp>
#include "Shape/Simulation/Systems/GroupBehaviorSystem.h"
#include "ECS/World.hpp"
#include "ECS/FlockComponent.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"

TEST_CASE("GroupBehaviorSystem - Boids Steering & Flocking", "[Simulation][GroupBehavior]") {
    Shape::World world;
    Shape::Simulation::GroupBehaviorSystem group_sys(&world);
    Shape::Simulation::DeterministicRng rng(42);
    Shape::TickContext ctx{1, 1.0 / 60.0, 0.016, rng};

    SECTION("System metadata") {
        REQUIRE(group_sys.name() == "GroupBehaviorSystem");
        REQUIRE(group_sys.priority() == 112);
    }

    SECTION("Separation, Alignment, and Cohesion calculation") {
        Shape::TransformComponent t_self;
        t_self.position = {0.0f, 0.0f};

        Shape::RigidBodyComponent rb_self;
        rb_self.velocity = {10.0f, 0.0f};

        std::vector<std::pair<Shape::TransformComponent, Shape::RigidBodyComponent>> neighbors;

        Shape::TransformComponent t_n1;
        t_n1.position = {2.0f, 0.0f}; // To the right
        Shape::RigidBodyComponent rb_n1;
        rb_n1.velocity = {0.0f, 20.0f}; // Heading up

        neighbors.emplace_back(t_n1, rb_n1);

        Shape::Math::Vector2f sep = Shape::Simulation::GroupBehaviorSystem::compute_separation(t_self, neighbors, 50.0f);
        Shape::Math::Vector2f ali = Shape::Simulation::GroupBehaviorSystem::compute_alignment(rb_self, neighbors);
        Shape::Math::Vector2f coh = Shape::Simulation::GroupBehaviorSystem::compute_cohesion(t_self, neighbors);

        REQUIRE(sep.x < 0.0f); // Pushes left away from neighbor at (2,0)
        REQUIRE(ali.y == 20.0f); // Aligns with neighbor's Y velocity
        REQUIRE(coh.x == 2.0f);  // Steers toward neighbor position (2,0)
    }

    SECTION("Flock update cycle applies forces to boids") {
        Shape::Entity boid1 = world.CreateEntity();
        Shape::Entity boid2 = world.CreateEntity();

        Shape::FlockComponent f1;
        f1.flock_id = 1;

        Shape::TransformComponent t1;
        t1.position = {0.0f, 0.0f};
        Shape::RigidBodyComponent rb1;

        Shape::TransformComponent t2;
        t2.position = {5.0f, 0.0f};
        Shape::RigidBodyComponent rb2;

        world.AddComponent(boid1, f1);
        world.AddComponent(boid1, t1);
        world.AddComponent(boid1, rb1);

        world.AddComponent(boid2, f1);
        world.AddComponent(boid2, t2);
        world.AddComponent(boid2, rb2);

        group_sys.update(ctx);

        auto& updated_rb1 = world.GetComponent<Shape::RigidBodyComponent>(boid1);
        // Force applied to boid1 should steer away from boid2 (separation) and toward boid2 (cohesion)
        REQUIRE(updated_rb1.force.LengthSquared() > 0.0f);
    }
}
