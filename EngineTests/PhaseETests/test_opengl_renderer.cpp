#include <catch2/catch_test_macros.hpp>
#include "Rendering/OpenGLRenderer.hpp"
#include "Rendering/RenderingSystem.hpp"
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include "Physics/ColliderComponent.hpp"

TEST_CASE("OpenGLRenderer - Instance Data & Struct Alignment", "[Rendering][OpenGL]") {
    SECTION("InstanceData struct memory layout & size") {
        REQUIRE(sizeof(Shape::InstanceData) == 36); // Vec2 pos(8) + rot(4) + Vec2 scale(8) + RenderColor(16)
        
        Shape::InstanceData inst;
        inst.position = {10.0f, -20.0f};
        inst.rotation = 1.57f;
        inst.scale = {2.0f, 2.0f};
        inst.color = {1.0f, 0.5f, 0.0f, 1.0f};

        REQUIRE(inst.position.x == 10.0f);
        REQUIRE(inst.position.y == -20.0f);
        REQUIRE(inst.rotation == 1.57f);
        REQUIRE(inst.color.r == 1.0f);
    }

    SECTION("RenderingSystem Query & Instancing Batching Logic") {
        Shape::World world;
        Shape::Entity e1 = world.CreateEntity();
        
        Shape::TransformComponent trans;
        trans.position = {100.0f, 50.0f};
        trans.scale = {1.5f, 1.5f};
        world.AddComponent<Shape::TransformComponent>(e1, trans);

        Shape::GeometryComponent geom;
        geom.vertices = {{-1.0f, -1.0f}, {1.0f, -1.0f}, {0.0f, 1.0f}};
        world.AddComponent<Shape::GeometryComponent>(e1, geom);

        auto polyEntities = world.Query<Shape::TransformComponent, Shape::GeometryComponent>();
        REQUIRE(polyEntities.size() == 1);
    }
}
