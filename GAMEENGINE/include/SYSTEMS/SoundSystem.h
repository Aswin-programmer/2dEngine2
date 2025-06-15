// Engine integration example (SoundSystem.h)
#pragma once

class AudioEvent;
class Registry;

class SoundSystem
{
public:
	SoundSystem() = default;
	~SoundSystem() = default;

	// System initialization and shutdown
	bool Initialize();
	void Shutdown();

	// Bank loading
	bool LoadBanks( const std::string& banksFolder );
	bool LoadBank( const std::string& bankName, const std::string& bankPath );

	// Event playback
	std::shared_ptr<AudioEvent> CreateEvent( const std::string& eventPath );
	bool PlayOneShot( const std::string& eventPath, float x = 0.0f, float y = 0.0f );

	// Listener position (for game's camera or player)
	void SetListenerPosition( float x, float y );

	// Global parameters (like game state, environment, etc.)
	bool SetGlobalParameter( const std::string& name, float value );
	float GetGlobalParameter( const std::string& name );

	// Mixing control
	bool SetBusVolume( const std::string& busPath, float volume );
	bool SetVCAVolume( const std::string& vcaPath, float volume );

	// Snapshot control (for transitions, special effects, etc.)
	bool StartSnapshot( const std::string& snapshotPath );
	bool StopSnapshot( const std::string& snapshotPath );

	// FMOD Studio timeline/time-based playback control
	std::shared_ptr<AudioEvent> PlayMusicTrack( const std::string& musicEventPath );
	bool StopAllMusic( bool allowFadeOut = true );

	// Update function to be called every frame
	void Update( float deltaTime, Registry& registry );

	void UpdateEntityPosition( Registry& registry );
	void UpdateListenerPosition( Registry& registry );

	static void CreateLuaSoundSystemBind( sol::state& lua, Registry& registry );

private:
	// Keep track of created events
	std::vector<std::shared_ptr<AudioEvent>> m_ActiveEvents;
	std::shared_ptr<AudioEvent> m_CurrentMusicTrack = nullptr;

	// Clean up destroyed or finished events
	void CleanupEvents();

	// Time since last FMOD update
	float m_TimeSinceLastUpdate = 0.0f;
};
