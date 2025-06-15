/**
 * @file AudioEvent.h
 * @brief A higher-level wrapper for FMOD event instances.
 *
 * @details This file provides a simplified interface for working with FMOD Studio events,
 * offering easy playback control, parameter manipulation, and positional audio support.
 * The AudioEvent class wraps FMOD::Studio::EventInstance to provide a more user-friendly
 * API for game audio management.
 */

#pragma once

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
	AudioEvent( const std::string& eventPath );
	~AudioEvent();

	// Basic playback control
	bool Play();
	bool Stop( bool allowFadeOut = true );
	bool Pause( bool pause = true );
	bool Resume( bool pause = false );
	bool IsPaused() const;

	// Positional audio
	void SetPosition( float x, float y );

	// Parameter control
	bool SetParameter( const std::string& name, float value );
	float GetParameter( const std::string& name ) const;

	// Volume control
	bool SetVolume( float volume );
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
