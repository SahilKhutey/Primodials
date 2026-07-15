#include "ECS2/CommandBuffer2.hpp"
#include "ECS2/World2.hpp"

namespace Shape::ECS2 {

EntityId CommandBuffer2::create() {
    Command cmd;
    cmd.type = CommandType::CreateEntity;
    // Use a temporary placeholder index; resolved to real EntityId on flush.
    cmd.entity = EntityId{0x80000000u + m_pending_creates, 0};
    m_commands.push_back(std::move(cmd));
    ++m_pending_creates;
    return m_commands.back().entity; // Placeholder — not yet valid in world
}

void CommandBuffer2::destroy(EntityId e) {
    Command cmd;
    cmd.type   = CommandType::DestroyEntity;
    cmd.entity = e;
    m_commands.push_back(std::move(cmd));
}

void CommandBuffer2::flush(World2& world) {
    // Resolve placeholder EntityIds from pending creates
    // (commands that reference placeholders use index 0x80000000+n)
    std::vector<EntityId> created_ids;
    created_ids.reserve(m_pending_creates);

    auto resolve = [&](EntityId e) -> EntityId {
        if (e.index >= 0x80000000u) {
            u32 idx = e.index - 0x80000000u;
            if (idx < created_ids.size()) return created_ids[idx];
        }
        return e;
    };

    for (auto& cmd : m_commands) {
        switch (cmd.type) {
            case CommandType::CreateEntity: {
                EntityId real = world.create();
                created_ids.push_back(real);
                break;
            }
            case CommandType::DestroyEntity:
                world.cmd_destroy(resolve(cmd.entity));
                break;
            case CommandType::AddComponent:
                world.cmd_add_raw(resolve(cmd.entity), cmd.cid,
                                   cmd.data.data(), cmd.size, cmd.ops);
                // Destruct the captured copy after use
                cmd.ops.destruct(cmd.data.data());
                cmd.data.clear();
                break;
            case CommandType::RemoveComponent:
                world.cmd_remove_raw(resolve(cmd.entity), cmd.cid);
                break;
        }
    }

    clear();
}

} // namespace Shape::ECS2
