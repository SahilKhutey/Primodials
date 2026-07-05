#include "EntityInspector.hpp"
#include "ECS/TransformComponent.hpp"
#include "Simulation/ReplaySystem.hpp"
#include "imgui.h"
#include <string>
#include <iostream>
#include <iostream>

namespace ShapeEditor {

void EntityInspector::Render(Shape::World& world) {
    ImGui::Begin("Entity Inspector");

    const auto& stores = world.GetComponentStores();
    
    // We specifically look for the TransformComponent store
    Shape::IComponentStore* transformStoreRaw = nullptr;
    for (const auto& [type_index, store_ptr] : stores) {
        if (store_ptr->GetComponentTypeId() == static_cast<u32>(Shape::ComponentTypeId::TransformComponent)) {
            transformStoreRaw = store_ptr.get();
            break;
        }
    }

    if (transformStoreRaw) {
        auto* typedStore = static_cast<Shape::ComponentStore<Shape::TransformComponent>*>(transformStoreRaw);
        const Shape::Entity* entities = typedStore->GetEntities();
        const Shape::TransformComponent* components = typedStore->GetData();
        size_t count = typedStore->GetSize();

        if (ImGui::TreeNode("Transform Components")) {
            for (size_t i = 0; i < count; ++i) {
                Shape::Entity e = entities[i];
                const auto& comp = components[i];
                
                std::string nodeName = "Entity " + std::to_string(static_cast<u32>(e));
                if (ImGui::TreeNode(nodeName.c_str())) {
                    ImGui::Text("Position: (%.2f, %.2f)", comp.position.x, comp.position.y);
                    ImGui::Text("Rotation: %.2f", comp.rotation);
                    ImGui::Text("Scale: (%.2f, %.2f)", comp.scale.x, comp.scale.y);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::Text("No Transform Components found.");
    }

    ImGui::End();

    ImGui::Begin("Replay Controls");
    static int currentTick = 0;
    ImGui::Text("Keyframes Stored: %llu", static_cast<unsigned long long>(Shape::ReplaySystem::GetKeyframeCount()));
    if (ImGui::Button("Capture Snapshot")) {
        Shape::ReplaySystem::CaptureKeyframe(world, static_cast<u64>(currentTick));
        currentTick++;
    }
    
    static int restoreTick = 0;
    ImGui::InputInt("Target Tick", &restoreTick);
    if (ImGui::Button("Restore Snapshot")) {
        if (restoreTick >= 0) {
            Shape::ReplaySystem::RestoreKeyframe(world, static_cast<u64>(restoreTick));
        }
    }
    if (ImGui::Button("Clear Snapshots")) {
        Shape::ReplaySystem::ClearKeyframes();
    }
    ImGui::End();
}

} // namespace ShapeEditor
