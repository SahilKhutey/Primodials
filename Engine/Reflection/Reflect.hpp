#pragma once

/// @file TypeId.hpp — Compile-time sequential type ID for Reflection.
/// @file TypeRegistry.hpp — Runtime name→id and id→name mapping.
/// @file Property.hpp — Named field descriptor (offset + type).
/// @file Reflect.hpp — SHAPE_REFLECT macro for component registration.
///
/// These four are combined into one header for Phase B simplicity.
/// Split into separate files in Phase C if complexity warrants it.

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <string_view>
#include <vector>
#include <unordered_map>

namespace Shape::Reflection {

// ─── TypeId ──────────────────────────────────────────────────────────────────

using TypeId = u32;
static constexpr TypeId INVALID_TYPE_ID = ~u32{0};

namespace detail {
    inline u32 next_type_id() noexcept { static u32 c = 0; return c++; }
}

template<typename T>
TypeId type_id() noexcept {
    static const TypeId id = detail::next_type_id();
    return id;
}

// ─── Property ────────────────────────────────────────────────────────────────

struct Property {
    const char* name;
    TypeId      type;    ///< TypeId of the field's own type
    usize       offset;  ///< offsetof(T, field)
    usize       size;    ///< sizeof(field)
};

// ─── TypeDescriptor ──────────────────────────────────────────────────────────

struct TypeDescriptor {
    TypeId              id;
    const char*         name;
    usize               size;
    usize               alignment;
    std::vector<Property> properties;
};

// ─── TypeRegistry ─────────────────────────────────────────────────────────────

class TypeRegistry {
public:
    static TypeRegistry& instance() noexcept {
        static TypeRegistry r;
        return r;
    }

    void register_type(TypeDescriptor desc) {
        const TypeId id = desc.id;
        m_by_name[desc.name] = id;
        if (id >= m_types.size()) m_types.resize(id + 1);
        m_types[id] = std::move(desc);
    }

    const TypeDescriptor* find(TypeId id) const noexcept {
        if (id >= m_types.size()) return nullptr;
        return &m_types[id];
    }

    const TypeDescriptor* find(std::string_view name) const noexcept {
        auto it = m_by_name.find(std::string(name));
        if (it == m_by_name.end()) return nullptr;
        return find(it->second);
    }

private:
    TypeRegistry() = default;
    std::vector<TypeDescriptor>              m_types;
    std::unordered_map<std::string, TypeId>  m_by_name;
};

// ─── SHAPE_REFLECT macro ──────────────────────────────────────────────────────

/// Register a type and its fields with the Reflection system.
/// Usage:
///   struct Transform { float x, y, rotation; };
///   SHAPE_REFLECT(Transform, x, y, rotation);
///
/// Must be called at program startup (e.g., in a global ctor or main()).

#define SHAPE_REFLECT(Type, ...)                                             \
    do {                                                                      \
        ::Shape::Reflection::TypeDescriptor _desc;                           \
        _desc.id        = ::Shape::Reflection::type_id<Type>();              \
        _desc.name      = #Type;                                              \
        _desc.size      = sizeof(Type);                                       \
        _desc.alignment = alignof(Type);                                      \
        ::Shape::Reflection::detail::reflect_fields<Type>(_desc, __VA_ARGS__);\
        ::Shape::Reflection::TypeRegistry::instance().register_type(          \
            std::move(_desc));                                                \
    } while(false)

namespace detail {
    template<typename T>
    void reflect_fields(TypeDescriptor&) {}

    template<typename T, typename Field, typename... Rest>
    void reflect_fields(TypeDescriptor& desc, const char* name, Field T::* member, Rest... rest) {
        Property p;
        p.name   = name;
        p.type   = type_id<Field>();
        p.offset = reinterpret_cast<usize>(&(static_cast<T*>(nullptr)->*member));
        p.size   = sizeof(Field);
        desc.properties.push_back(p);
        reflect_fields<T>(desc, rest...);
    }
}

} // namespace Shape::Reflection
