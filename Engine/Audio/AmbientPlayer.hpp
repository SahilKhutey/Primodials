#pragma once

#include "Core/Platform.hpp"
#include <string>

namespace Shape::Audio {

class AmbientPlayer {
public:
    AmbientPlayer() = default;
    ~AmbientPlayer() = default;

    bool Initialize();
    void Shutdown();

    void PlayThemeAudio(const std::string& audioPath, f32 targetVolume = 0.3f, f32 crossfadeDuration = 3.0f);
    void Update(f32 dt);

    void SetMasterVolume(f32 volume0to1);
    f32 GetMasterVolume() const { return m_MasterVolume; }

    bool IsPlaying() const { return m_IsPlaying; }
    std::string GetCurrentAudioPath() const { return m_CurrentAudioPath; }

private:
    std::string m_CurrentAudioPath;
    std::string m_TargetAudioPath;
    f32 m_MasterVolume = 0.5f;
    f32 m_CurrentVolume = 0.0f;
    f32 m_TargetVolume = 0.3f;
    f32 m_CrossfadeDuration = 3.0f;
    f32 m_CrossfadeProgress = 1.0f;
    bool m_IsPlaying = false;
    bool m_InCrossfade = false;
};

} // namespace Shape::Audio
