// SoundSystem.cpp
#include "SoundSystem.h"
#include <filesystem>
#include <iostream>

bool SoundSystem::Initialize()
{
    return FMODAudioSystem::GetInstance().Initialize();
}

void SoundSystem::Shutdown()
{
    // Stop and clear all active events
    m_ActiveEvents.clear();
    m_CurrentMusicTrack = nullptr;

    // Shutdown FMOD
    FMODAudioSystem::GetInstance().Shutdown();
}

bool SoundSystem::LoadBanks(const std::string& banksFolder)
{
    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(banksFolder))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                std::string extension = entry.path().extension().string();

                // Only load .bank files
                if (extension == ".bank")
                {
                    std::string bankName = entry.path().stem().string();
                    std::string bankPath = entry.path().string();

                    if (!LoadBank(bankName, bankPath))
                    {
                        std::cerr << "Failed to load bank: " << bankName << std::endl;
                    }
                }
            }
        }
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading banks: " << e.what() << std::endl;
        return false;
    }
}

bool SoundSystem::LoadBank(const std::string& bankName, const std::string& bankPath)
{
    return FMODAudioSystem::GetInstance().LoadBank(bankName, bankPath);
}

std::shared_ptr<AudioEvent> SoundSystem::CreateEvent(const std::string& eventPath)
{
    auto event = std::make_shared<AudioEvent>(eventPath);
    if (event->IsValid())
    {
        m_ActiveEvents.push_back(event);
        return event;
    }

    return nullptr;
}

bool SoundSystem::PlayOneShot(const std::string& eventPath, float x, float y)
{
    FMOD_VECTOR position = { x, y, 0 };
    return FMODAudioSystem::GetInstance().PlayOneShot(eventPath, position);
}

void SoundSystem::SetListenerPosition(float x, float y)
{
    FMODAudioSystem::GetInstance().Set3DListenerPosition(x, y);
}

bool SoundSystem::SetGlobalParameter(const std::string& name, float value)
{
    return FMODAudioSystem::GetInstance().SetGlobalParameter(name, value);
}

float SoundSystem::GetGlobalParameter(const std::string& name)
{
    return FMODAudioSystem::GetInstance().GetGlobalParameter(name);
}

bool SoundSystem::SetBusVolume(const std::string& busPath, float volume)
{
    return FMODAudioSystem::GetInstance().SetBusVolume(busPath, volume);
}

bool SoundSystem::SetVCAVolume(const std::string& vcaPath, float volume)
{
    return FMODAudioSystem::GetInstance().SetVCAVolume(vcaPath, volume);
}

bool SoundSystem::StartSnapshot(const std::string& snapshotPath)
{
    return FMODAudioSystem::GetInstance().StartSnapshot(snapshotPath);
}

bool SoundSystem::StopSnapshot(const std::string& snapshotPath)
{
    return FMODAudioSystem::GetInstance().StopSnapshot(snapshotPath);
}

std::shared_ptr<AudioEvent> SoundSystem::PlayMusicTrack(const std::string& musicEventPath)
{
    // Stop current music first
    StopAllMusic();

    // Create and play the new music track
    m_CurrentMusicTrack = CreateEvent(musicEventPath);
    if (m_CurrentMusicTrack)
    {
        m_CurrentMusicTrack->Play();
    }

    return m_CurrentMusicTrack;
}

bool SoundSystem::StopAllMusic(bool allowFadeOut)
{
    if (m_CurrentMusicTrack)
    {
        bool result = m_CurrentMusicTrack->Stop(allowFadeOut);
        m_CurrentMusicTrack = nullptr;
        return result;
    }

    return true;
}

void SoundSystem::Update(float deltaTime, Registry& registry)
{
    UpdateEntitesPosition(registry);
    UpdateListenerPosition(registry);

    // Accumulate delta time
    m_TimeSinceLastUpdate += deltaTime;

    // Update FMOD at fixed intervals (e.g., 60Hz)
    const float updateInterval = 1.0f / 60.0f;
    if (m_TimeSinceLastUpdate >= updateInterval)
    {
        // Update FMOD
        FMODAudioSystem::GetInstance().Update();

        // Clean up finished events
        CleanupEvents();

        // Reset timer
        m_TimeSinceLastUpdate = 0.0f;
    }
}

void SoundSystem::UpdateEntitesPosition(Registry& registry)
{
    registry.GetRegistry().view<TransformComponent, SoundEmitter>().each
    (
        [](auto entity, TransformComponent& transformCoponent, SoundEmitter& soundComponent)
        {
            if (soundComponent.audioEvent)
            {
                soundComponent.audioEvent->SetPosition(
                    transformCoponent.position.x,
                    -transformCoponent.position.y
                );
            }
        }
    );
}

void SoundSystem::UpdateListenerPosition(Registry& registry)
{
    auto soundSystem = registry.GetContext<std::shared_ptr<SoundSystem>>();
    registry.GetRegistry().view<TransformComponent, SoundListener>().each
    (
        [soundSystem](auto entity, TransformComponent& transformCoponent, SoundListener& soundListener)
        {
            if (soundListener.isActive)
            {
                LOG_INFO("x :'{0}' y :'{1}'", transformCoponent.position.x, -transformCoponent.position.y)
                soundSystem->SetListenerPosition(
                    transformCoponent.position.x,
                    -transformCoponent.position.y
                );
            }
        }
    );
}

void SoundSystem::CreateLuaSoundSystemBind(sol::state& lua, Registry& registry)
{
    auto soundSystem = registry.GetContext<std::shared_ptr<SoundSystem>>();

    lua.new_usertype<SoundSystem>(
        "SoundSystem",
        sol::no_constructor,

        // Bank loading
        "load_banks", [](SoundSystem& self, const std::string& banksFolder) {
            return self.LoadBanks(banksFolder);
        },
        "load_bank", [](SoundSystem& self, const std::string& bankName, const std::string& bankPath) {
            return self.LoadBank(bankName, bankPath);
        },
        "play_one_shot", [](SoundSystem& self, const std::string& eventPath, float x, float y) {
            return self.PlayOneShot(eventPath, x, y);
        },
        // Global parameters
        "set_global_parameter", [](SoundSystem& self, const std::string& name, float value) {
            return self.SetGlobalParameter(name, value);
        },
        "get_global_parameter", [](SoundSystem& self, const std::string& name) -> float {
            return self.GetGlobalParameter(name);
        },

        // Mixing control
        "set_bus_volume", [](SoundSystem& self, const std::string& busPath, float volume) {
            return self.SetBusVolume(busPath, volume);
        },
        "set_vca_volume", [](SoundSystem& self, const std::string& vcaPath, float volume) {
            return self.SetVCAVolume(vcaPath, volume);
        },

        // Snapshot control
        "start_snapshot", [](SoundSystem& self, const std::string& snapshotPath) {
            return self.StartSnapshot(snapshotPath);
        },
        "stop_snapshot", [](SoundSystem& self, const std::string& snapshotPath) {
            return self.StopSnapshot(snapshotPath);
        },

        //// Music control
        //"play_music_track", [](SoundSystem& self, const std::string& musicEventPath) {
        //    return self.PlayMusicTrack(musicEventPath);
        //},
        "stop_all_music", [](SoundSystem& self, bool allowFadeOut) {
            return self.StopAllMusic(allowFadeOut);
        }
    );
}

void SoundSystem::CleanupEvents()
{
    // Remove any events that are no longer active or valid
    m_ActiveEvents.erase(
        std::remove_if(m_ActiveEvents.begin(), m_ActiveEvents.end(),
            [](const std::shared_ptr<AudioEvent>& event) {
                return !event->IsValid() || !event->IsPlaying();
            }),
        m_ActiveEvents.end()
    );

    // Check if music track is still playing
    if (m_CurrentMusicTrack && (!m_CurrentMusicTrack->IsValid() || !m_CurrentMusicTrack->IsPlaying()))
    {
        m_CurrentMusicTrack = nullptr;
    }
}