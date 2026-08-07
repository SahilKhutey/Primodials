#pragma once

#include "VisualScripting/Core/DataType.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include <variant>
#include <string>

namespace Shape::VisualScripting {

using ValueVariant = std::variant<
    std::monostate,
    bool,
    i64,
    f64,
    std::string,
    Math::Vector2f,
    u64
>;

class Value {
public:
    Value() = default;
    Value(bool v) : m_data(v), m_type(DataType::Bool) {}
    Value(i32 v) : m_data(static_cast<i64>(v)), m_type(DataType::Int) {}
    Value(i64 v) : m_data(v), m_type(DataType::Int) {}
    Value(f32 v) : m_data(static_cast<f64>(v)), m_type(DataType::Float) {}
    Value(f64 v) : m_data(v), m_type(DataType::Float) {}
    Value(const std::string& v) : m_data(v), m_type(DataType::String) {}
    Value(const Math::Vector2f& v) : m_data(v), m_type(DataType::Vec2) {}
    Value(u64 v) : m_data(v), m_type(DataType::Entity) {}

    DataType GetType() const { return m_type; }
    bool IsValid() const { return m_type != DataType::Invalid && !std::holds_alternative<std::monostate>(m_data); }

    bool AsBool() const { return std::holds_alternative<bool>(m_data) ? std::get<bool>(m_data) : false; }
    i64 AsInt() const { return std::holds_alternative<i64>(m_data) ? std::get<i64>(m_data) : 0; }
    f64 AsFloat() const {
        if (std::holds_alternative<f64>(m_data)) return std::get<f64>(m_data);
        if (std::holds_alternative<i64>(m_data)) return static_cast<f64>(std::get<i64>(m_data));
        return 0.0;
    }
    std::string AsString() const { return std::holds_alternative<std::string>(m_data) ? std::get<std::string>(m_data) : ""; }
    Math::Vector2f AsVec2() const { return std::holds_alternative<Math::Vector2f>(m_data) ? std::get<Math::Vector2f>(m_data) : Math::Vector2f{ 0.0f, 0.0f }; }
    u64 AsEntity() const { return std::holds_alternative<u64>(m_data) ? std::get<u64>(m_data) : 0; }

    std::string ToString() const {
        if (!IsValid()) return "null";
        switch (m_type) {
            case DataType::Bool:   return AsBool() ? "true" : "false";
            case DataType::Int:    return std::to_string(AsInt());
            case DataType::Float:  return std::to_string(AsFloat());
            case DataType::String: return AsString();
            case DataType::Vec2:   return "(" + std::to_string(AsVec2().x) + ", " + std::to_string(AsVec2().y) + ")";
            case DataType::Entity: return "Entity(" + std::to_string(AsEntity()) + ")";
            default:               return "?";
        }
    }

private:
    ValueVariant m_data;
    DataType m_type = DataType::Invalid;
};

} // namespace Shape::VisualScripting
