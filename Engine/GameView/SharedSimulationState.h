#pragma once

#include <memory>
#include <mutex>
#include <atomic>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>

namespace ShapeEngine::GameView {

    class SharedSimulationState {
    public:
        enum class AccessMode {
            SingleProcess,
            MultiProcess,
        };

        struct Snapshot {
            std::vector<uint8_t> worldData;
            uint64_t tickNumber = 0;
            uint32_t entityCount = 0;
            std::array<uint32_t, 32> speciesCounts{};
            float totalPopulation = 0;
            uint64_t lastUpdateTimestamp = 0;
        };

        static SharedSimulationState& get();

        void initialize(AccessMode mode = AccessMode::SingleProcess);
        void shutdown();

        Snapshot takeSnapshot();
        void restoreFromSnapshot(const Snapshot& snapshot);

        bool hasUpdatedSince(uint64_t lastTimestamp) const;

        void requestPause(bool paused);
        bool isPauseRequested() const;

        void requestSpeed(float speed);
        float getRequestedSpeed() const;

        void applyParameterChange(const std::string& paramName, float value);
        void injectInput(const std::string& inputType, const std::string& payload);

        void setTickStats(uint64_t tickNumber, uint32_t entityCount, const std::array<uint32_t, 32>& speciesCounts);

    private:
        SharedSimulationState() = default;

        AccessMode m_mode = AccessMode::SingleProcess;
        mutable std::mutex m_mutex;

        std::atomic<bool> m_pauseRequested{ false };
        std::atomic<float> m_speedOverride{ 1.0f };

        std::atomic<uint64_t> m_updateCounter{ 0 };
        std::atomic<uint64_t> m_lastSnapshotTimestamp{ 0 };

        uint64_t m_currentTick = 0;
        uint32_t m_currentEntityCount = 0;
        std::array<uint32_t, 32> m_currentSpeciesCounts{};
    };

} // namespace ShapeEngine::GameView
