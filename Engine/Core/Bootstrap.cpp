#include "Core/Bootstrap.hpp"
#include "Jobs/JobSystem.hpp"
#include "Core/Logger.hpp"

#include <iostream>
#include <chrono>

namespace shape {

Engine::Engine() : m_commandBuffer(m_world) {}
Engine::~Engine() {}

Engine& Engine::Get() {
    static Engine instance;
    return instance;
}

bool Engine::Initialize() {
    if (m_initialized) {
        return true;
    }

    SHAPE_LOG_INFO("Bootstrap: Initializing Engine Core modules...");
    JobSystem::Get().Initialize();
    
    // Wire up Simulation Systems to Phase 2 (Update/Physics)
    m_scheduler.RegisterPhase2([this]() {
        f32 dt = m_scheduler.GetClock().GetTimeStepSeconds();
        m_cognitiveSystem.Update(m_world, dt);
        m_civilizationSystem.Update(m_world, dt);
        m_biologySystem.Update(m_world, m_commandBuffer, dt, m_frameCounter++);
        m_physicsSystem.Update(m_world, dt);
    });

    // Wire up ECS command buffer flushing to Phase 3 of the simulation tick
    m_scheduler.RegisterPhase3([this]() {
        m_commandBuffer.Flush();
        m_latestStateHash = m_desyncVerifier.CalculateStateHash(m_world);
    });

    m_initialized = true;
    m_running = true;
    return true;
}

void Engine::Run(u32 maxFrames) {
    if (!m_initialized || !m_running) {
        return;
    }

    SHAPE_LOG_INFO("Bootstrap: Running Engine Loop...");
    
    auto lastTime = std::chrono::high_resolution_clock::now();
    u32 framesRun = 0;
    
    while (m_running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        u64 deltaUs = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count();
        lastTime = currentTime;

        m_scheduler.Update(deltaUs);

        if (maxFrames > 0) {
            framesRun++;
            if (framesRun >= maxFrames) {
                break;
            }
        }
    }
}

void Engine::Shutdown() {
    if (!m_initialized) {
        return;
    }

    SHAPE_LOG_INFO("Bootstrap: Shutting down Engine modules...");
    m_initialized = false;
    m_running = false;
    JobSystem::Get().Shutdown();
}

} // namespace shape
