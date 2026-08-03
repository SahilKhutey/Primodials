// Shape/Scripting/IScriptEngine.h
#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

namespace Shape::Scripting {

    enum class ScriptValueType : uint8_t {
        Nil, Bool, Number, String, Table, Function, Userdata
    };

    class ScriptEngine;

    class ScriptValue {
    public:
        ScriptValue() = default;
        // ... opaque
    };

    using ScriptFunction = std::function<int(ScriptEngine&)>;

    class ScriptEngine {
    public:
        virtual ~ScriptEngine() = default;
        virtual bool initialize() = 0;
        virtual void shutdown() = 0;
        virtual void update(double delta_time) = 0;
        virtual bool execute_file(const std::string& path) = 0;
        virtual bool execute_string(const std::string& code) = 0;
    };

} // namespace Shape::Scripting
