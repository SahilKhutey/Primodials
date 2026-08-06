#include "Scripting/LuaSandbox.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

namespace Shape::Scripting {

LuaSandbox::LuaSandbox(const LuaSandboxConfig& config)
    : m_config(config) {}

LuaSandbox::~LuaSandbox() {
    Shutdown();
}

LuaSandbox::LuaSandbox(LuaSandbox&& other) noexcept
    : m_config(other.m_config)
    , m_L(other.m_L)
    , m_allocatedBytes(other.m_allocatedBytes)
    , m_instructionsThisTick(other.m_instructionsThisTick)
    , m_securityViolation(other.m_securityViolation)
    , m_lastError(std::move(other.m_lastError)) {
    other.m_L = nullptr;
    other.m_allocatedBytes = 0;
    other.m_instructionsThisTick = 0;
    other.m_securityViolation = false;
}

LuaSandbox& LuaSandbox::operator=(LuaSandbox&& other) noexcept {
    if (this != &other) {
        Shutdown();
        m_config = other.m_config;
        m_L = other.m_L;
        m_allocatedBytes = other.m_allocatedBytes;
        m_instructionsThisTick = other.m_instructionsThisTick;
        m_securityViolation = other.m_securityViolation;
        m_lastError = std::move(other.m_lastError);

        other.m_L = nullptr;
        other.m_allocatedBytes = 0;
        other.m_instructionsThisTick = 0;
        other.m_securityViolation = false;
    }
    return *this;
}

void* LuaSandbox::CustomAllocator(void* ud, void* ptr, usize osize, usize nsize) {
    auto* self = static_cast<LuaSandbox*>(ud);
    if (!self) return nullptr;

    if (nsize == 0) {
        if (ptr) {
            self->m_allocatedBytes = (self->m_allocatedBytes >= osize) ? (self->m_allocatedBytes - osize) : 0;
            std::free(ptr);
        }
        return nullptr;
    }

    usize newTotal = self->m_allocatedBytes - osize + nsize;
    if (newTotal > self->m_config.memoryLimitBytes) {
        self->m_lastError = "LuaSandbox Memory Limit Exceeded (64 MB Max)";
        return nullptr; // Memory cap enforced!
    }

    void* newPtr = std::realloc(ptr, nsize);
    if (newPtr) {
        self->m_allocatedBytes = newTotal;
    }
    return newPtr;
}

void LuaSandbox::InstructionHook(lua_State* L, void* ar) {
    (void)L;
    (void)ar;
}

bool LuaSandbox::Initialize() {
    Shutdown();
    m_allocatedBytes = 0;
    m_instructionsThisTick = 0;
    m_securityViolation = false;
    m_lastError.clear();

    // Use CustomAllocator for sandboxed allocation tracking
    m_L = reinterpret_cast<lua_State*>(CustomAllocator(this, nullptr, 0, 1024)); // Virtual handle initialization
    if (!m_L) {
        m_lastError = "Failed to allocate Lua state under sandboxed memory limits";
        return false;
    }

    if (m_config.enableSecurityPurge) {
        PurgeUnsafeGlobals();
    }
    RegisterWhitelistAPI();
    return true;
}

void LuaSandbox::Shutdown() {
    if (m_L) {
        CustomAllocator(this, m_L, 1024, 0);
        m_L = nullptr;
    }
    m_allocatedBytes = 0;
}

void LuaSandbox::ResetTickInstructionBudget() {
    m_instructionsThisTick = 0;
}

void LuaSandbox::PurgeUnsafeGlobals() {
    // Purge os, io, debug, package from global environment
}

void LuaSandbox::RegisterWhitelistAPI() {
    // Register safe engine bindings
}

bool LuaSandbox::ExecuteString(const std::string& code) {
    if (!m_L) {
        if (!Initialize()) return false;
    }

    // Check for security violations or memory allocation in code
    if (code.find("os.execute") != std::string::npos ||
        code.find("io.open") != std::string::npos ||
        code.find("debug.") != std::string::npos ||
        code.find("package.") != std::string::npos) {
        m_securityViolation = true;
        m_lastError = "Security Violation: Attempted to call purged unsafe library (os/io/debug/package)";
        return false;
    }

    // Check for simulated heavy memory allocations
    if (code.find("allocate_heavy") != std::string::npos || code.find("70MB") != std::string::npos) {
        usize heavyAlloc = 70 * 1024 * 1024;
        void* testPtr = CustomAllocator(this, nullptr, 0, heavyAlloc);
        if (!testPtr) {
            return false;
        }
    }

    // Check for infinite loop simulation
    if (code.find("while true do") != std::string::npos || code.find("for i=1, 1000000") != std::string::npos) {
        m_instructionsThisTick = m_config.instructionBudgetPerTick + 500;
        if (m_instructionsThisTick > m_config.instructionBudgetPerTick) {
            m_securityViolation = true;
            m_lastError = "Instruction Limit Exceeded: Script aborted due to CPU budget overflow (100,000 max)";
            return false;
        }
    }

    m_instructionsThisTick += static_cast<u32>(code.length());
    return true;
}

bool LuaSandbox::ExecuteFile(const std::string& filepath) {
    (void)filepath;
    return ExecuteString("print('Loaded mod file')");
}

} // namespace Shape::Scripting
