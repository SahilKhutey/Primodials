#pragma once

#include "Network/Protocol/Packet.hpp"
#include "ShapeEngine/Math/Vector2.hpp"

namespace Shape::Network {

enum class PlayerInputType : u8 {
    None = 0,
    SpawnEntity = 1,
    DespawnEntity = 2,
    SelectEntity = 3,
    ParameterChange = 6
};

struct PlayerInput {
    PlayerInputType type = PlayerInputType::None;
    u32 tickNumber = 0;
    u64 playerId = 0;
    u32 targetEntity = 0;
    Math::Vector2f position = { 0.0f, 0.0f };
    f32 floatValue = 0.0f;
    std::string stringValue;
};

class InputBatch {
public:
    u32 startTick = 0;
    u32 endTick = 0;
    u32 frameNumber = 0;
    std::vector<PlayerInput> inputs;

    std::vector<u8> Serialize() const {
        PacketBuilder b(PacketType::InputBatch);
        b.WriteU32(startTick);
        b.WriteU32(endTick);
        b.WriteU32(frameNumber);
        b.WriteU32(static_cast<u32>(inputs.size()));

        for (const auto& in : inputs) {
            b.WriteU8(static_cast<u8>(in.type));
            b.WriteU32(in.tickNumber);
            b.WriteU64(in.playerId);
            b.WriteU32(in.targetEntity);
            b.WriteF32(in.position.x);
            b.WriteF32(in.position.y);
            b.WriteF32(in.floatValue);
            b.WriteString(in.stringValue);
        }

        return b.Build();
    }

    static InputBatch Deserialize(PacketReader& r) {
        InputBatch batch;
        batch.startTick = r.ReadU32();
        batch.endTick = r.ReadU32();
        batch.frameNumber = r.ReadU32();
        u32 count = r.ReadU32();

        batch.inputs.reserve(count);
        for (u32 i = 0; i < count; ++i) {
            PlayerInput in;
            in.type = static_cast<PlayerInputType>(r.ReadU8());
            in.tickNumber = r.ReadU32();
            in.playerId = r.ReadU64();
            in.targetEntity = r.ReadU32();
            in.position.x = r.ReadF32();
            in.position.y = r.ReadF32();
            in.floatValue = r.ReadF32();
            in.stringValue = r.ReadString();
            batch.inputs.push_back(in);
        }

        return batch;
    }
};

} // namespace Shape::Network
