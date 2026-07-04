#include "Serializer.hpp"
#include "ECS/ComponentTypes.hpp"
#include <fstream>
#include <iostream>
#include "Core/Assert.hpp"

// Include all components
#include "ECS/AIComponent.hpp"
#include "ECS/CareerComponent.hpp"
#include "ECS/CivilizationComponent.hpp"
#include "ECS/GeneticsComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include "ECS/InfluenceComponent.hpp"
#include "ECS/LifecycleComponent.hpp"
#include "ECS/MindComponent.hpp"
#include "ECS/ResourceComponent.hpp"
#include "ECS/SentinelComponent.hpp"
#include "ECS/StorageComponent.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"

namespace Shape {

constexpr u32 MAGIC_HEADER = 0x53484150; // "SHAP"
constexpr u32 CURRENT_VERSION = 1;

bool Serializer::SaveWorld(const World& world, const std::string& filepath) {
    std::ofstream os(filepath, std::ios::binary);
    if (!os.is_open()) return false;
    return SaveWorld(world, os);
}

bool Serializer::SaveWorld(const World& world, std::ostream& os) {
    // Header
    os.write(reinterpret_cast<const char*>(&MAGIC_HEADER), sizeof(MAGIC_HEADER));
    os.write(reinterpret_cast<const char*>(&CURRENT_VERSION), sizeof(CURRENT_VERSION));

    // World Entities
    world.SerializeEntities(os);

    // Component Stores
    const auto& stores = world.GetComponentStores();
    u32 numStores = static_cast<u32>(stores.size());
    os.write(reinterpret_cast<const char*>(&numStores), sizeof(numStores));

    for (const auto& [typeIndex, store] : stores) {
        u32 typeId = store->GetComponentTypeId();
        os.write(reinterpret_cast<const char*>(&typeId), sizeof(typeId));
        store->Serialize(os);
    }

    return os.good();
}

bool Serializer::LoadWorld(World& world, const std::string& filepath) {
    std::ifstream is(filepath, std::ios::binary);
    if (!is.is_open()) return false;
    return LoadWorld(world, is);
}

bool Serializer::LoadWorld(World& world, std::istream& is) {
    u32 magic = 0;
    u32 version = 0;
    is.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    is.read(reinterpret_cast<char*>(&version), sizeof(version));

    if (magic != MAGIC_HEADER || version != CURRENT_VERSION) {
        return false;
    }

    world.DeserializeEntities(is);

    u32 numStores = 0;
    is.read(reinterpret_cast<char*>(&numStores), sizeof(numStores));

    for (u32 i = 0; i < numStores; ++i) {
        u32 typeId = 0;
        is.read(reinterpret_cast<char*>(&typeId), sizeof(typeId));

        switch (static_cast<ComponentTypeId>(typeId)) {
        case ComponentTypeId::AIComponent:
            world.EnsureComponentStore<AIComponent>();
            world.GetComponentStore<AIComponent>().Deserialize(is);
            break;
        case ComponentTypeId::CareerComponent:
            world.EnsureComponentStore<CareerComponent>();
            world.GetComponentStore<CareerComponent>().Deserialize(is);
            break;
        case ComponentTypeId::CivilizationComponent:
            world.EnsureComponentStore<CivilizationComponent>();
            world.GetComponentStore<CivilizationComponent>().Deserialize(is);
            break;
        case ComponentTypeId::GeneticsComponent:
            world.EnsureComponentStore<GeneticsComponent>();
            world.GetComponentStore<GeneticsComponent>().Deserialize(is);
            break;
        case ComponentTypeId::GeometryComponent:
            world.EnsureComponentStore<GeometryComponent>();
            world.GetComponentStore<GeometryComponent>().Deserialize(is);
            break;
        case ComponentTypeId::InfluenceComponent:
            world.EnsureComponentStore<InfluenceComponent>();
            world.GetComponentStore<InfluenceComponent>().Deserialize(is);
            break;
        case ComponentTypeId::LifecycleComponent:
            world.EnsureComponentStore<LifecycleComponent>();
            world.GetComponentStore<LifecycleComponent>().Deserialize(is);
            break;
        case ComponentTypeId::MindComponent:
            world.EnsureComponentStore<MindComponent>();
            world.GetComponentStore<MindComponent>().Deserialize(is);
            break;
        case ComponentTypeId::ResourceComponent:
            world.EnsureComponentStore<ResourceComponent>();
            world.GetComponentStore<ResourceComponent>().Deserialize(is);
            break;
        case ComponentTypeId::SentinelComponent:
            world.EnsureComponentStore<SentinelComponent>();
            world.GetComponentStore<SentinelComponent>().Deserialize(is);
            break;
        case ComponentTypeId::StorageComponent:
            world.EnsureComponentStore<StorageComponent>();
            world.GetComponentStore<StorageComponent>().Deserialize(is);
            break;
        case ComponentTypeId::TransformComponent:
            world.EnsureComponentStore<TransformComponent>();
            world.GetComponentStore<TransformComponent>().Deserialize(is);
            break;
        case ComponentTypeId::ColliderComponent:
            world.EnsureComponentStore<ColliderComponent>();
            world.GetComponentStore<ColliderComponent>().Deserialize(is);
            break;
        case ComponentTypeId::RigidBodyComponent:
            world.EnsureComponentStore<RigidBodyComponent>();
            world.GetComponentStore<RigidBodyComponent>().Deserialize(is);
            break;
        default:
            SHAPE_ASSERT_MSG(false, "Unknown component TypeId encountered during deserialization!");
            break;
        }
    }

    return is.good();
}

} // namespace Shape
