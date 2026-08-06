#pragma once

#include "Core/Platform.hpp"
#include "ECS/World.hpp"
#include "Serialization/SimSerializer.hpp"
#include <vector>
#include <string>

namespace Shape::Simulation {

struct ReplayFrame {
    u64 tickIndex = 0;
    f64 timestamp = 0.0;
    std::vector<u8> snapshotData;
};

class ReplaySystem {
public:
    ReplaySystem() = default;
    ~ReplaySystem() = default;

    void RecordFrame(World& world, u64 tickIndex, f64 timestamp);
    bool RestoreFrame(World& world, usize frameIndex);

    void Clear();

    void SetRecording(bool recording) { m_isRecording = recording; }
    bool IsRecording() const { return m_isRecording; }

    void SetPlaybackSpeed(f32 speed) { m_playbackSpeed = speed; }
    f32 GetPlaybackSpeed() const { return m_playbackSpeed; }

    usize GetFrameCount() const { return m_frames.size(); }
    const ReplayFrame* GetFrame(usize index) const {
        return index < m_frames.size() ? &m_frames[index] : nullptr;
    }

    // Static keyframe convenience interface
    static void CaptureKeyframe(World& world, u64 tickIndex);
    static bool RestoreKeyframe(World& world, u64 tickIndex);
    static void ClearKeyframes();
    static usize GetKeyframeCount();

private:
    bool m_isRecording = true;
    f32 m_playbackSpeed = 1.0f;
    std::vector<ReplayFrame> m_frames;

    static ReplaySystem s_globalInstance;
};

} // namespace Shape::Simulation
