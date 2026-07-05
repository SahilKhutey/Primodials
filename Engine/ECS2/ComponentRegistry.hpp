#pragma once

/// @file ComponentRegistry.hpp
/// @brief Global registry mapping ComponentId → TypeInfo.
///
/// Usage:
///   ComponentRegistry::register_type<Transform>("Transform");
///   const TypeInfo& info = ComponentRegistry::get(component_id<Transform>());

#include "ECS2/TypeInfo.hpp"
#include "ECS2/ComponentId.hpp"
#include "Core/Assert.hpp"
#include <array>
#include <string_view>

namespace Shape::ECS2 {

class ComponentRegistry {
public:
    ComponentRegistry()  = delete;
    ~ComponentRegistry() = delete;

    /// Register component type T. Must be called before any World2 that uses T.
    /// Safe to call multiple times for the same T (idempotent).
    template<typename T>
    static const TypeInfo& register_type(const char* name = nullptr) noexcept {
        const ComponentId id = component_id<T>();
        SHAPE_ASSERT_MSG(id < MAX_COMPONENT_TYPES,
            "Too many component types registered (max {})", MAX_COMPONENT_TYPES);

        TypeInfo& info = s_registry[id];
        if (info.size == 0) { // Not yet registered
            info.id        = id;
            info.size      = sizeof(T);
            info.alignment = alignof(T);
            info.name      = name ? name : "Component";
            info.ops       = make_type_ops<T>();
            ++s_count;
        }
        return info;
    }

    /// Retrieve TypeInfo for a registered component.
    SHAPE_NODISCARD static const TypeInfo& get(ComponentId id) noexcept {
        SHAPE_ASSERT(id < MAX_COMPONENT_TYPES && s_registry[id].size > 0);
        return s_registry[id];
    }

    /// Check if a component type is registered.
    SHAPE_NODISCARD static bool is_registered(ComponentId id) noexcept {
        return id < MAX_COMPONENT_TYPES && s_registry[id].size > 0;
    }

    SHAPE_NODISCARD static usize registered_count() noexcept { return s_count; }

private:
    inline static std::array<TypeInfo, MAX_COMPONENT_TYPES> s_registry{};
    inline static usize s_count = 0;
};

/// Convenience macro — register + get name from type.
/// Usage: SHAPE_REGISTER_COMPONENT(Transform);
#define SHAPE_REGISTER_COMPONENT(T) \
    ::Shape::ECS2::ComponentRegistry::register_type<T>(#T)

} // namespace Shape::ECS2
