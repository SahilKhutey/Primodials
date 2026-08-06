// Engine/GameView/SharedStateSync.h
#pragma once

#include "ECS/World.hpp"
#include <memory>
#include <mutex>
#include <atomic>
#include <array>
#include <vector>
#include <string>

namespace ShapeEngine::GameView {

    /// <summary>
    /// Synchronizes simulation state between wallpaper process and 
    /// game view process (or between two game views for multiplayer).
    /// </summary>
    class SharedStateSync {
    public:
        enum class SyncMode {
            SingleProcess,
            MultiProcess,
        };

        struct Snapshot {
            uint64_t tickNumber = 0;
            uint32_t entityCount = 0;
            std::array<uint32_t, 32> speciesCounts{};
            float avgEnergy = 0.0f;
            float avgAge = 0.0f;
            uint64_t timestamp = 0;
            
            std::vector<uint8_t> serializedWorld;
            std::vector<std::string> pendingCommands;
        };

        struct Command {
            enum class Type {
                SpawnEntity,
                DespawnEntity,
                ModifyTrait,
                PauseSimulation,
                ResumeSimulation,
                ChangeSpeed,
                ApplyTheme,
                SaveSnapshot,
                LoadSnapshot,
                ResetSimulation
            };
            Type type = Type::PauseSimulation;
            std::string param1;
            std::string param2;
            float floatParam = 0.0f;
            int intParam = 0;
        };

        SharedStateSync();
        ~SharedStateSync();

        void initialize(Shape::World* world, SyncMode mode = SyncMode::SingleProcess);
        void shutdown();

        Snapshot takeLightSnapshot();
        Snapshot takeFullSnapshot();
        void applySnapshot(const Snapshot& snapshot);

        bool hasRemoteUpdates() const;

        void queueCommand(const Command& cmd);
        std::vector<Command> getPendingCommands();
        void clearCommands();

        bool isMultiProcess() const { return m_mode == SyncMode::MultiProcess; }
        SyncMode getMode() const { return m_mode; }

    private:
        void initializeMultiProcess();
        void pollMultiProcessUpdates();
        void sendCommandMultiProcess(const Command& cmd);

        Shape::World* m_world = nullptr;
        SyncMode m_mode = SyncMode::SingleProcess;
        mutable std::mutex m_mutex;
        std::atomic<bool> m_hasUpdates{false};
        std::atomic<uint64_t> m_lastSyncTimestamp{0};
        
        void* m_sharedMemory = nullptr;
        size_t m_sharedMemorySize = 0;
        std::string m_sharedMemoryName;
        
        std::vector<Command> m_commandQueue;
        mutable std::mutex m_commandMutex;
    };

} // namespace ShapeEngine::GameView
