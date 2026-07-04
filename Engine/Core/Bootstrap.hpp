#pragma once

#include "Core/Platform.hpp"
#include "Simulation/SimulationScheduler.hpp"
#include "ECS/World.hpp"
#include "ECS/CommandBuffer.hpp"

#include "Physics/PhysicsSystem.hpp"
#include "Biology/BiologySystem.hpp"
#include "AI/CognitiveSystem.hpp"
#include "Civilization/CivilizationSystem.hpp"
#include "Networking/LockstepSynchronizer.hpp"
#include "Networking/DesyncVerifier.hpp"

namespace shape {

class Engine {
public:
    static Engine& Get();

    bool Initialize();
    void Run(u32 maxFrames = 0);
    void Shutdown();

    bool IsRunning() const { return m_running; }
    void Update(u64 deltaUs) { m_scheduler.Update(deltaUs); }
    Shape::World& GetWorld() { return m_world; }
    f32 GetAlpha() const { return m_scheduler.GetAlpha(); }
    u32 GetLatestStateHash() const { return m_latestStateHash; }

    Engine();
    ~Engine();

private:
    bool m_running = false;
    bool m_initialized = false;
    
    Shape::SimulationScheduler m_scheduler;
    Shape::World m_world;
    Shape::CommandBuffer m_commandBuffer;

    Shape::PhysicsSystem m_physicsSystem;
    Shape::BiologySystem m_biologySystem;
    Shape::CognitiveSystem m_cognitiveSystem;
    Shape::CivilizationSystem m_civilizationSystem;
    Shape::LockstepSynchronizer m_lockstepSync;
    Shape::DesyncVerifier m_desyncVerifier;
    u32 m_latestStateHash = 0;
    u32 m_frameCounter = 0;
};

} // namespace shape
