#include "Audio/AmbientPlayer.hpp"
#include "Core/Logger.hpp"

namespace Shape::Audio {

bool AmbientPlayer::Initialize() {
    m_MasterVolume = 0.5f;
    m_CurrentVolume = 0.0f;
    m_TargetVolume = 0.3f;
    m_IsPlaying = false;
    m_InCrossfade = false;
    SHAPE_LOG_INFO("AmbientPlayer initialized.");
    return true;
}

void AmbientPlayer::Shutdown() {
    m_IsPlaying = false;
    m_InCrossfade = false;
}

void AmbientPlayer::PlayThemeAudio(const std::string& audioPath, f32 targetVolume, f32 crossfadeDuration) {
    if (m_CurrentAudioPath == audioPath && m_IsPlaying) {
        m_TargetVolume = targetVolume;
        return;
    }

    m_TargetAudioPath = audioPath;
    m_TargetVolume = targetVolume;
    m_CrossfadeDuration = (crossfadeDuration > 0.01f) ? crossfadeDuration : 0.01f;
    m_CrossfadeProgress = 0.0f;
    m_InCrossfade = true;

    SHAPE_LOG_INFO("AmbientPlayer: Transitioning track to '{}' (Volume: {:.2f}, Crossfade: {:.1f}s)",
                   audioPath, targetVolume, m_CrossfadeDuration);
}

void AmbientPlayer::Update(f32 dt) {
    if (!m_InCrossfade) return;

    m_CrossfadeProgress += dt / m_CrossfadeDuration;
    if (m_CrossfadeProgress >= 1.0f) {
        m_CrossfadeProgress = 1.0f;
        m_InCrossfade = false;
        m_CurrentAudioPath = m_TargetAudioPath;
        m_CurrentVolume = m_TargetVolume;
        m_IsPlaying = !m_CurrentAudioPath.empty();
        return;
    }

    // Linear crossfade calculation
    m_CurrentVolume = m_TargetVolume * m_CrossfadeProgress;
}

void AmbientPlayer::SetMasterVolume(f32 volume0to1) {
    m_MasterVolume = (volume0to1 < 0.0f) ? 0.0f : ((volume0to1 > 1.0f) ? 1.0f : volume0to1);
}

} // namespace Shape::Audio
