#pragma once

#include "Core/Platform.hpp"
#include <string>

namespace Shape::VisualScripting {

enum class DataType : u16 {
    Invalid   = 0,
    Bool      = 1,
    Int       = 2,
    Float     = 3,
    String    = 5,
    Vec2      = 10,
    Entity    = 100,
    Exec      = 500,
    Any       = 600
};

inline const char* GetDataTypeName(DataType t) {
    switch (t) {
        case DataType::Bool:   return "Bool";
        case DataType::Int:    return "Int";
        case DataType::Float:  return "Float";
        case DataType::String: return "String";
        case DataType::Vec2:   return "Vec2";
        case DataType::Entity: return "Entity";
        case DataType::Exec:   return "▶";
        case DataType::Any:    return "Any";
        default:               return "Invalid";
    }
}

inline bool AreTypesCompatible(DataType a, DataType b) {
    if (a == b) return true;
    if (a == DataType::Any || b == DataType::Any) return true;
    if ((a == DataType::Int && b == DataType::Float) || (a == DataType::Float && b == DataType::Int)) return true;
    return false;
}

} // namespace Shape::VisualScripting
