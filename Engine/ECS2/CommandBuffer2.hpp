#pragma once

/// @file CommandBuffer2.hpp
/// @brief Deferred ECS2 structural changes — safe to issue during for_each iteration.
///
/// Structural changes (add/remove component, create/destroy entity) cannot be applied
/// mid-iteration without invalidating chunk pointers. CommandBuffer2 queues them
/// and replays them after the iteration completes.
///
/// Usage:
///   world.for_each<Velocity>([&](EntityId e, Velocity& v) {
///       if (v.speed < 0.01f) cmd.destroy(e);
///       else cmd.add<Tagged>(e, {});
///   });
///   cmd.flush(world);

#include "ECS2/EntityId.hpp"
#include "ECS2/ComponentId.hpp"
#include "ECS2/TypeInfo.hpp"
#include "Core/Assert.hpp"
#include <functional>
#include <vector>

namespace Shape::ECS2 {

class World2; // forward

class CommandBuffer2 {
public:
    /// Queue entity creation. Returns a placeholder EntityId (resolved on flush).
    EntityId create();

    /// Queue entity destruction.
    void destroy(EntityId e);

    /// Queue component addition (value captured by copy).
    template<typename T>
    void add(EntityId e, T component = T{}) {
        const ComponentId cid = component_id<T>();
        // Capture the component value in a type-erased buffer
        Command cmd;
        cmd.type   = CommandType::AddComponent;
        cmd.entity = e;
        cmd.cid    = cid;
        cmd.size   = sizeof(T);
        cmd.data.resize(sizeof(T));
        new (cmd.data.data()) T{std::move(component)};
        cmd.ops = make_type_ops<T>();
        m_commands.push_back(std::move(cmd));
    }

    /// Queue component removal.
    template<typename T>
    void remove(EntityId e) {
        Command cmd;
        cmd.type   = CommandType::RemoveComponent;
        cmd.entity = e;
        cmd.cid    = component_id<T>();
        m_commands.push_back(std::move(cmd));
    }

    /// Replay all queued commands against world. Clears the buffer.
    void flush(World2& world);

    /// Discard all queued commands without applying.
    void clear() noexcept { m_commands.clear(); m_pending_creates = 0; }

    SHAPE_NODISCARD bool  empty()        const noexcept { return m_commands.empty(); }
    SHAPE_NODISCARD usize command_count() const noexcept { return m_commands.size(); }

private:
    enum class CommandType : u8 {
        CreateEntity,
        DestroyEntity,
        AddComponent,
        RemoveComponent
    };

    struct Command {
        CommandType       type   = CommandType::CreateEntity;
        EntityId          entity = EntityId::invalid();
        ComponentId       cid    = INVALID_COMPONENT_ID;
        usize             size   = 0;
        std::vector<u8>   data;  ///< Type-erased component value
        TypeOps           ops{};
    };

    std::vector<Command> m_commands;
    u32                  m_pending_creates = 0;
};

} // namespace Shape::ECS2
