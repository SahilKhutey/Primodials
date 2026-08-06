#include "Simulation/ReplaySystem.hpp"

namespace Shape::Simulation {

ReplaySystem ReplaySystem::s_globalInstance;

void ReplaySystem::RecordFrame(World& world, u64 tickIndex, f64 timestamp) {
    if (!m_isRecording) return;

    ReplayFrame frame;
    frame.tickIndex = tickIndex;
    frame.timestamp = timestamp;
    if (Serialization::SimSerializer::SaveToMemory(world, frame.snapshotData, timestamp)) {
        m_frames.push_back(std::move(frame));
    }
}

bool ReplaySystem::RestoreFrame(World& world, usize frameIndex) {
    if (frameIndex >= m_frames.size()) return false;
    return Serialization::SimSerializer::LoadFromMemory(world, m_frames[frameIndex].snapshotData);
}

void ReplaySystem::Clear() {
    m_frames.clear();
}

void ReplaySystem::CaptureKeyframe(World& world, u64 tickIndex) {
    s_globalInstance.RecordFrame(world, tickIndex, static_cast<f64>(tickIndex) * 0.016);
}

bool ReplaySystem::RestoreKeyframe(World& world, u64 tickIndex) {
    for (usize i = 0; i < s_globalInstance.m_frames.size(); ++i) {
        if (s_globalInstance.m_frames[i].tickIndex == tickIndex) {
            return s_globalInstance.RestoreFrame(world, i);
        }
    }
    return false;
}

void ReplaySystem::ClearKeyframes() {
    s_globalInstance.Clear();
}

usize ReplaySystem::GetKeyframeCount() {
    return s_globalInstance.GetFrameCount();
}

} // namespace Shape::Simulation
