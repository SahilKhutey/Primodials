#include "SharedSimulationState.h"
#include "Core/Logger.hpp"

namespace ShapeEngine::GameView {

    SharedSimulationState& SharedSimulationState::get() {
        static SharedSimulationState instance;
        return instance;
    }

    void SharedSimulationState::initialize(AccessMode mode) {
        m_mode = mode;
        SHAPE_LOG_INFO("SharedSimulationState initialized");
    }

    void SharedSimulationState::shutdown() {}

    void SharedSimulationState::setTickStats(uint64_t tickNumber, uint32_t entityCount, const std::array<uint32_t, 32>& speciesCounts) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_currentTick = tickNumber;
        m_currentEntityCount = entityCount;
        m_currentSpeciesCounts = speciesCounts;
        m_updateCounter++;
    }

    SharedSimulationState::Snapshot SharedSimulationState::takeSnapshot() {
        std::lock_guard<std::mutex> lock(m_mutex);
        Snapshot snap;
        snap.tickNumber = m_currentTick;
        snap.entityCount = m_currentEntityCount;
        snap.speciesCounts = m_currentSpeciesCounts;
        snap.totalPopulation = static_cast<float>(m_currentEntityCount);
        snap.lastUpdateTimestamp = m_updateCounter.load();
        m_lastSnapshotTimestamp.store(snap.lastUpdateTimestamp);
        return snap;
    }

    void SharedSimulationState::restoreFromSnapshot(const Snapshot& snapshot) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_currentTick = snapshot.tickNumber;
        m_currentEntityCount = snapshot.entityCount;
        m_currentSpeciesCounts = snapshot.speciesCounts;
    }

    bool SharedSimulationState::hasUpdatedSince(uint64_t lastTimestamp) const {
        return m_updateCounter.load() > lastTimestamp;
    }

    void SharedSimulationState::requestPause(bool paused) {
        m_pauseRequested.store(paused);
    }

    bool SharedSimulationState::isPauseRequested() const {
        return m_pauseRequested.load();
    }

    void SharedSimulationState::requestSpeed(float speed) {
        m_speedOverride.store(std::clamp(speed, 0.1f, 4.0f));
    }

    float SharedSimulationState::getRequestedSpeed() const {
        return m_speedOverride.load();
    }

    void SharedSimulationState::applyParameterChange(const std::string& paramName, float value) {
        (void)paramName;
        (void)value;
    }

    void SharedSimulationState::injectInput(const std::string& inputType, const std::string& payload) {
        (void)inputType;
        (void)payload;
    }

} // namespace ShapeEngine::GameView
