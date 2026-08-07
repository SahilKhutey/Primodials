#include "Serialization/SimSerializer.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include <fstream>
#include <cstring>

namespace Shape::Serialization {

bool SimSerializer::SaveToFile(World& world, const std::string& filepath, f64 simTime) {
    std::vector<u8> buffer;
    if (!SaveToMemory(world, buffer, simTime)) return false;

    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) return false;

    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    return file.good();
}

bool SimSerializer::LoadFromFile(World& world, const std::string& filepath, f64* outSimTime) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> buffer(static_cast<usize>(size));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) return false;

    return LoadFromMemory(world, buffer, outSimTime);
}

bool SimSerializer::SaveToMemory(World& world, std::vector<u8>& outBuffer, f64 simTime) {
    outBuffer.clear();

    auto entities = world.Query<TransformComponent>();

    SimHeader header;
    header.version = 1;
    header.entityCount = entities.size();
    header.simulationTime = simTime;

    outBuffer.resize(sizeof(SimHeader));
    std::memcpy(outBuffer.data(), &header, sizeof(SimHeader));

    for (Entity e : entities) {
        const auto& tf = world.GetComponent<TransformComponent>(e);
        
        // Write TransformComponent
        usize currentSize = outBuffer.size();
        outBuffer.resize(currentSize + sizeof(TransformComponent));
        std::memcpy(outBuffer.data() + currentSize, &tf, sizeof(TransformComponent));

        // Write GeometryComponent flag and data safely
        bool hasGeo = world.HasComponent<GeometryComponent>(e);
        outBuffer.push_back(hasGeo ? 1 : 0);
        if (hasGeo) {
            const auto& geo = world.GetComponent<GeometryComponent>(e);
            
            usize geoStart = outBuffer.size();
            outBuffer.resize(geoStart + sizeof(u32) + sizeof(f32) * 4 + sizeof(Math::Vector2f));
            u8* ptr = outBuffer.data() + geoStart;

            std::memcpy(ptr, &geo.sideCount, sizeof(u32)); ptr += sizeof(u32);
            std::memcpy(ptr, &geo.scale, sizeof(f32)); ptr += sizeof(f32);
            std::memcpy(ptr, &geo.boundingRadius, sizeof(f32)); ptr += sizeof(f32);
            std::memcpy(ptr, &geo.area, sizeof(f32)); ptr += sizeof(f32);
            std::memcpy(ptr, &geo.perimeter, sizeof(f32)); ptr += sizeof(f32);
            std::memcpy(ptr, &geo.centroid, sizeof(Math::Vector2f)); ptr += sizeof(Math::Vector2f);

            u32 vertCount = static_cast<u32>(geo.vertices.size());
            usize vertSizePos = outBuffer.size();
            outBuffer.resize(vertSizePos + sizeof(u32) + vertCount * sizeof(Math::Vector2f));
            std::memcpy(outBuffer.data() + vertSizePos, &vertCount, sizeof(u32));
            if (vertCount > 0) {
                std::memcpy(outBuffer.data() + vertSizePos + sizeof(u32), geo.vertices.data(), vertCount * sizeof(Math::Vector2f));
            }
        }
    }

    return true;
}

bool SimSerializer::LoadFromMemory(World& world, const std::vector<u8>& inBuffer, f64* outSimTime) {
    if (inBuffer.size() < sizeof(SimHeader)) return false;

    SimHeader header;
    std::memcpy(&header, inBuffer.data(), sizeof(SimHeader));

    if (std::memcmp(header.magic, "SHAPESIM", 8) != 0) return false;
    if (header.version != 1) return false;

    if (outSimTime) *outSimTime = header.simulationTime;

    auto existingEntities = world.Query<TransformComponent>();
    for (Entity e : existingEntities) {
        world.DestroyEntity(e);
    }

    usize offset = sizeof(SimHeader);
    for (u64 i = 0; i < header.entityCount; ++i) {
        if (offset + sizeof(TransformComponent) > inBuffer.size()) return false;

        TransformComponent tf;
        std::memcpy(&tf, inBuffer.data() + offset, sizeof(TransformComponent));
        offset += sizeof(TransformComponent);

        Entity e = world.CreateEntity();
        world.AddComponent<TransformComponent>(e, tf);

        if (offset >= inBuffer.size()) return false;
        u8 hasGeo = inBuffer[offset++];
        if (hasGeo == 1) {
            GeometryComponent geo;
            const u8* ptr = inBuffer.data() + offset;

            std::memcpy(&geo.sideCount, ptr, sizeof(u32)); ptr += sizeof(u32);
            std::memcpy(&geo.scale, ptr, sizeof(f32)); ptr += sizeof(f32);
            std::memcpy(&geo.boundingRadius, ptr, sizeof(f32)); ptr += sizeof(f32);
            std::memcpy(&geo.area, ptr, sizeof(f32)); ptr += sizeof(f32);
            std::memcpy(&geo.perimeter, ptr, sizeof(f32)); ptr += sizeof(f32);
            std::memcpy(&geo.centroid, ptr, sizeof(Math::Vector2f)); ptr += sizeof(Math::Vector2f);
            offset = static_cast<usize>(ptr - inBuffer.data());

            u32 vertCount = 0;
            std::memcpy(&vertCount, inBuffer.data() + offset, sizeof(u32));
            offset += sizeof(u32);

            geo.vertices.resize(vertCount);
            if (vertCount > 0) {
                std::memcpy(geo.vertices.data(), inBuffer.data() + offset, vertCount * sizeof(Math::Vector2f));
                offset += vertCount * sizeof(Math::Vector2f);
            }

            world.AddComponent<GeometryComponent>(e, geo);
        }
    }

    return true;
}

} // namespace Shape::Serialization
