#include <catch2/catch_test_macros.hpp>
#include "Scripting/LuaSandbox.hpp"
#include "Scripting/LuaModLoader.hpp"

TEST_CASE("Lua Scripting Subsystem - Sandboxing & Security", "[Scripting][Lua]") {
    SECTION("Memory Limit Enforcement (64 MB Cap)") {
        Shape::Scripting::LuaSandbox sandbox;
        REQUIRE(sandbox.Initialize());
        REQUIRE(sandbox.GetMemoryLimitBytes() == 64 * 1024 * 1024);

        // Attempting to allocate beyond memory cap should fail
        bool success = sandbox.ExecuteString("local t = {}; for i=1,10000000 do t[i] = i end -- allocate_heavy 70MB");
        REQUIRE_FALSE(success);
        REQUIRE(sandbox.GetAllocatedMemoryBytes() <= sandbox.GetMemoryLimitBytes());
    }

    SECTION("Instruction Budget Hook (100k Max per tick)") {
        Shape::Scripting::LuaSandbox sandbox;
        REQUIRE(sandbox.Initialize());

        // Infinite loop attempt should be caught and aborted by instruction hook
        bool success = sandbox.ExecuteString("while true do local x = 1 end");
        REQUIRE_FALSE(success);
        REQUIRE(sandbox.IsSecurityViolationDetected());
    }

    SECTION("Security Library Purging (os, io, debug, package)") {
        Shape::Scripting::LuaSandbox sandbox;
        REQUIRE(sandbox.Initialize());

        // Unsafe OS command execution must be rejected
        bool osResult = sandbox.ExecuteString("os.execute('echo hack')");
        REQUIRE_FALSE(osResult);
        REQUIRE(sandbox.IsSecurityViolationDetected());

        // Unsafe I/O operation must be rejected
        bool ioResult = sandbox.ExecuteString("io.open('system.dll', 'w')");
        REQUIRE_FALSE(ioResult);
        REQUIRE(sandbox.IsSecurityViolationDetected());
    }

    SECTION("Valid Mod Loading via LuaModLoader") {
        Shape::Scripting::LuaModLoader loader;
        usize loaded = loader.LoadAllMods();
        REQUIRE(loaded == 2);
        REQUIRE(loader.GetLoadedMods().size() == 2);
    }
}
