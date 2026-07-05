#include "Renderer2D.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"

namespace ShapeEditor {

Renderer2D::Renderer2D() {}

void Renderer2D::Render(Shape::World& world, SDL_Renderer* renderer) {
    const auto& stores = world.GetComponentStores();
    
    Shape::IComponentStore* transformStoreRaw = nullptr;
    for (const auto& [type_index, store_ptr] : stores) {
        if (store_ptr->GetComponentTypeId() == static_cast<u32>(Shape::ComponentTypeId::TransformComponent)) {
            transformStoreRaw = store_ptr.get();
            break;
        }
    }

    if (transformStoreRaw) {
        auto* typedStore = static_cast<Shape::ComponentStore<Shape::TransformComponent>*>(transformStoreRaw);
        const Shape::TransformComponent* components = typedStore->GetData();
        size_t count = typedStore->GetSize();
        
        SDL_SetRenderDrawColor(renderer, 50, 200, 100, 255); // Green-ish objects
        
        for (size_t i = 0; i < count; ++i) {
            Shape::Entity e = typedStore->GetEntities()[i];
            const auto& comp = components[i];
            
            float radius = 10.0f; // Default visual size
            if (world.HasComponent<Shape::ColliderComponent>(e)) {
                const auto& col = world.GetComponent<Shape::ColliderComponent>(e);
                if (col.type == Shape::ColliderType::Circle) {
                    radius = col.radius;
                } else if (col.type == Shape::ColliderType::Polygon) {
                    radius = 15.0f; // Rough approx
                }
            }
            
            // Apply camera transformation
            float screenX = (comp.position.x - cameraOffset.x) * cameraZoom + 640.0f; // center screen X
            float screenY = (comp.position.y - cameraOffset.y) * cameraZoom + 360.0f; // center screen Y
            float scaledW = 2.0f * radius * comp.scale.x * cameraZoom;
            float scaledH = 2.0f * radius * comp.scale.y * cameraZoom;
            
            SDL_FRect rect = {
                screenX - scaledW * 0.5f,
                screenY - scaledH * 0.5f,
                scaledW,
                scaledH
            };
            
            SDL_RenderFillRect(renderer, &rect);
            
            if (world.HasComponent<Shape::RigidBodyComponent>(e)) {
                const auto& rb = world.GetComponent<Shape::RigidBodyComponent>(e);
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderLine(renderer, screenX, screenY, screenX + rb.velocity.x * 0.5f * cameraZoom, screenY + rb.velocity.y * 0.5f * cameraZoom);
                SDL_SetRenderDrawColor(renderer, 50, 200, 100, 255);
            }
        }
    }
}

} // namespace ShapeEditor
