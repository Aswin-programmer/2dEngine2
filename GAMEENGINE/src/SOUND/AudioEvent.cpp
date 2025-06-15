#include "pch.h"

#include "SOUND/AudioEvent.h"
#include "SOUND/FMODAudioSystem.h"

AudioEvent::AudioEvent( const std::string& eventPath )
	: m_EventPath( eventPath )
{
	m_EventInstance = FMODAudioSystem::GetInstance().CreateEventInstance( eventPath );
	m_IsValid = ( m_EventInstance != nullptr );
}

AudioEvent::~AudioEvent()
{
	if ( m_IsValid && m_EventInstance )
	{
		// Stop immediately (no fade out)
		Stop( false );

		// Release the event instance
		FMODAudioSystem::GetInstance().ReleaseEvent( m_EventInstance );
		m_EventInstance = nullptr;
	}
}

bool AudioEvent::Play()
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return false;
	}

	FMOD_RESULT result = m_EventInstance->start();
	return ( result == FMOD_OK );
}

bool AudioEvent::Stop( bool allowFadeOut )
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return false;
	}

	FMOD_STUDIO_STOP_MODE mode = allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
	FMOD_RESULT result = m_EventInstance->stop( mode );
	return ( result == FMOD_OK );
}

bool AudioEvent::Pause( bool pause )
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return false;
	}

	FMOD_RESULT result = m_EventInstance->setPaused( pause );
	return ( result == FMOD_OK );
}

bool AudioEvent::Resume( bool pause )
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return false;
	}

	FMOD_RESULT result = m_EventInstance->setPaused( pause );
	return ( result == FMOD_OK );
}

bool AudioEvent::IsPaused() const
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return false;
	}

	bool paused = false;
	FMOD_RESULT result = m_EventInstance->getPaused( &paused );
	return ( result == FMOD_OK ) && paused;
}

void AudioEvent::SetPosition( float x, float y )
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return;
	}

	FMODAudioSystem::GetInstance().Set3DEventPosition( m_EventInstance, x, y );
}

bool AudioEvent::SetParameter( const std::string& name, float value )
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return false;
	}

	return FMODAudioSystem::GetInstance().SetEventParameter( m_EventInstance, name, value );
}

float AudioEvent::GetParameter( const std::string& name ) const
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return 0.0f;
	}

	return FMODAudioSystem::GetInstance().GetEventParameter( m_EventInstance, name );
}

bool AudioEvent::SetVolume( float volume )
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return false;
	}

	FMOD_RESULT result = m_EventInstance->setVolume( volume );
	return ( result == FMOD_OK );
}

float AudioEvent::GetVolume() const
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return 0.0f;
	}

	float volume = 0.0f;
	FMOD_RESULT result = m_EventInstance->getVolume( &volume );
	return ( result == FMOD_OK ) ? volume : 0.0f;
}

bool AudioEvent::IsPlaying() const
{
	if ( !m_IsValid || !m_EventInstance )
	{
		return false;
	}

	FMOD_STUDIO_PLAYBACK_STATE state;
	FMOD_RESULT result = m_EventInstance->getPlaybackState( &state );

	return ( result == FMOD_OK ) && ( state == FMOD_STUDIO_PLAYBACK_PLAYING );
}

bool AudioEvent::IsValid() const
{
	return m_IsValid && ( m_EventInstance != nullptr );
}
