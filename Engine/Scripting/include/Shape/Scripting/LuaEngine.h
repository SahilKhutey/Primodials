#pragma once
#include "Shape/Scripting/IScriptEngine.h"
namespace Shape::Scripting {
    class LuaEngine : public ScriptEngine {
    public:
        bool initialize() override { return true; }
        void shutdown() override {}
        void update(double) override {}
        bool execute_file(const std::string&) override { return true; }
        bool execute_string(const std::string&) override { return true; }
    };
}
