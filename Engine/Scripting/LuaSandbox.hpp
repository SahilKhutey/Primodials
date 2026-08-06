#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <vector>
#include <functional>
#include <memory>

struct lua_State;

namespace Shape::Scripting {

struct LuaSandboxConfig {
    usize memoryLimitBytes = 64 * 1024 * 1024; // 64 MB cap
    u32 instructionBudgetPerTick = 100000;     // 100k instructions limit per tick
    bool enableSecurityPurge = true;          // Purge os, io, debug, package
};

class LuaSandbox {
public:
    explicit LuaSandbox(const LuaSandboxConfig& config = {});
    ~LuaSandbox();

    LuaSandbox(const LuaSandbox&) = delete;
    LuaSandbox& operator=(const LuaSandbox&) = delete;
    LuaSandbox(LuaSandbox&&) noexcept;
    LuaSandbox& operator=(LuaSandbox&&) noexcept;

    bool Initialize();
    void Shutdown();

    bool ExecuteString(const std::string& code);
    bool ExecuteFile(const std::string& filepath);

    void ResetTickInstructionBudget();
    
    usize GetAllocatedMemoryBytes() const { return m_allocatedBytes; }
    usize GetMemoryLimitBytes() const { return m_config.memoryLimitBytes; }
    u32 GetInstructionBudget() const { return m_config.instructionBudgetPerTick; }
    u32 GetInstructionsExecutedThisTick() const { return m_instructionsThisTick; }

    bool IsSecurityViolationDetected() const { return m_securityViolation; }
    const std::string& GetLastError() const { return m_lastError; }

    lua_State* GetLuaState() const { return m_L; }

private:
    static void* CustomAllocator(void* ud, void* ptr, usize osize, usize nsize);
    static void InstructionHook(lua_State* L, void* ar);

    void RegisterWhitelistAPI();
    void PurgeUnsafeGlobals();

    LuaSandboxConfig m_config;
    lua_State* m_L = nullptr;
    usize m_allocatedBytes = 0;
    u32 m_instructionsThisTick = 0;
    bool m_securityViolation = false;
    std::string m_lastError;
};

} // namespace Shape::Scripting
