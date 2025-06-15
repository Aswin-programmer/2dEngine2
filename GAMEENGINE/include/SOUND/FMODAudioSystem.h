/**
 * @file FMODAudioSystem.h
 * @brief FMOD Audio System Manager - Singleton wrapper for FMOD Studio and Core APIs
 *
 * @details This file provides a comprehensive interface for managing FMOD audio functionality,
 * including bank loading, event management, positional audio, parameter control, and mixing.
 * The system handles both FMOD Studio events and Core API sounds through a unified interface.
 */

#pragma once

/**
 * @class FMODAudioSystem
 * @brief Singleton manager for FMOD audio functionality
 *
 * Provides a high-level interface for FMOD Studio and Core APIs, handling
 * system initialization, bank management, event creation, and audio playback.
 */
class FMODAudioSystem
{
public:
	static FMODAudioSystem& GetInstance();

	// Delete copy and move constructors and assignment operators
	FMODAudioSystem( const FMODAudioSystem& ) = delete;
	FMODAudioSystem& operator=( const FMODAudioSystem& ) = delete;
	FMODAudioSystem( FMODAudioSystem&& ) = delete;
	FMODAudioSystem& operator=( FMODAudioSystem&& ) = delete;

	// System initialization and shutdown
	bool Initialize( int maxChannels = 32, int studioFlags = FMOD_STUDIO_INIT_NORMAL, int coreFlags = FMOD_INIT_NORMAL );
	void Shutdown();

	// Bank loading
	bool LoadBank( const std::string& bankName, const std::string& bankPath );
	bool UnloadBank( const std::string& bankName );

	// Events
	FMOD::Studio::EventInstance* CreateEventInstance( const std::string& eventPath );
	bool ReleaseEvent( FMOD::Studio::EventInstance* eventInstance );

	// Sound playback (for simple one-shot sounds)
	FMOD::Sound* LoadSound( const std::string& soundPath, bool loop = false );
	FMOD::Channel* PlaySound( FMOD::Sound* sound, float volume = 1.0f );
	bool PlayOneShot( const std::string& eventPath, const FMOD_VECTOR& position = { 0, 0, 0 } );

	// 2D positional audio
	void Set3DListenerPosition( float x, float y );
	void Set3DEventPosition( FMOD::Studio::EventInstance* eventInstance, float x, float y );

	// Parameter control
	bool SetEventParameter( FMOD::Studio::EventInstance* eventInstance, const std::string& parameterName, float value );
	float GetEventParameter( FMOD::Studio::EventInstance* eventInstance, const std::string& parameterName );

	// Global parameter control
	bool SetGlobalParameter( const std::string& parameterName, float value );
	float GetGlobalParameter( const std::string& parameterName );

	// Mixing and buses
	FMOD::Studio::Bus* GetBus( const std::string& busPath );
	bool SetBusVolume( const std::string& busPath, float volume );
	float GetBusVolume( const std::string& busPath );
	bool SetBusPaused( const std::string& busPath, bool paused );
	bool GetBusPaused( const std::string& busPath );

	// VCA control
	FMOD::Studio::VCA* GetVCA( const std::string& vcaPath );
	bool SetVCAVolume( const std::string& vcaPath, float volume );
	float GetVCAVolume( const std::string& vcaPath );

	// Runtime snapshot control
	bool StartSnapshot( const std::string& snapshotPath );
	bool StopSnapshot( const std::string& snapshotPath );

	// Update (call this every frame)
	void Update();

	// Direct access to FMOD systems (use carefully)
	FMOD::Studio::System* GetStudioSystem() const { return m_StudioSystem; }
	FMOD::System* GetCoreSystem() const { return m_CoreSystem; }

private:
	FMODAudioSystem() = default;
	~FMODAudioSystem() = default;

	FMOD_RESULT ErrorCheck( FMOD_RESULT result ) const;

	// FMOD systems
	FMOD::Studio::System* m_StudioSystem = nullptr;
	FMOD::System* m_CoreSystem = nullptr;

	// Banks
	std::map<std::string, FMOD::Studio::Bank*> m_Banks;

	// Cached sounds
	std::map<std::string, FMOD::Sound*> m_Sounds;

	// Cached event descriptions
	std::map<std::string, FMOD::Studio::EventDescription*> m_EventDescriptions;

	// Active snapshots
	std::map<std::string, FMOD::Studio::EventInstance*> m_ActiveSnapshots;

	// Initialization state
	bool m_Initialized = false;
};
