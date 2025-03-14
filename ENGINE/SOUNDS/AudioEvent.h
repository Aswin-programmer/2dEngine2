// AudioEvent.h - A higher-level wrapper for FMOD event instances
#pragma once
#include <iostream>

#include "FMODAudioSystem.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <sol/sol.hpp>
#include <entt/entt.hpp>

// Forward declarations
namespace FMOD
{
    namespace Studio
    {
        class EventInstance;
    }
}

// A higher-level wrapper for FMOD event instances
class AudioEvent
{
public:
    AudioEvent(const std::string& eventPath);
    ~AudioEvent();

    // Basic playback control
    bool Play();
    bool Stop(bool allowFadeOut = true);
    bool Pause(bool pause = true);
    bool Resume(bool pause = false);
    bool IsPaused() const;

    // Positional audio
    void SetPosition(float x, float y);

    // Parameter control
    bool SetParameter(const std::string& name, float value);
    float GetParameter(const std::string& name) const;

    // Volume control
    bool SetVolume(float volume);
    float GetVolume() const;

    // Event state
    bool IsPlaying() const;
    bool IsValid() const;

    // Get the raw FMOD event instance (use carefully)
    FMOD::Studio::EventInstance* GetRawEventInstance() const { return m_EventInstance; }

private:
    FMOD::Studio::EventInstance* m_EventInstance = nullptr;
    std::string m_EventPath;
    bool m_IsValid = false;
};