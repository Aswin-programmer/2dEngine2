#include "pch.h"

#include "SOUNDS/ESSENTIALS/OpenALSound.h"

#include "LOGGER/log.h"

OpenALSound::OpenALSound()
	: OpenALSound( 0 )
{
}

OpenALSound::OpenALSound( const ALuint buffer )
	: m_Source{ 0 }, m_Buffer{ buffer }, m_Pitch{ 1.f }, m_Gain{ 1.f }, m_Position{ 0.f, 0.f, 0.f }, m_Velocity{ 0.f, 0.f, 0.f }, m_LoopSound{ false }
{
	alGenSources( 1, &m_Source );
	alSourcef( m_Source, AL_PITCH, m_Pitch );
	alSourcef( m_Source, AL_GAIN, m_Gain );
	alSource3f( m_Source, AL_POSITION, m_Position[0], m_Position[1], m_Position[2] );
	alSource3f( m_Source, AL_VELOCITY, m_Velocity[0], m_Velocity[1], m_Velocity[2] );
	alSourcei( m_Source, AL_LOOPING, m_LoopSound );
}

OpenALSound::~OpenALSound()
{
	Stop();
	alDeleteSources( 1, &m_Source );
}

void OpenALSound::Play() const
{
	if ( m_Buffer == 0 )
	{
		LOG_WARN( "Failed to play sound. Buffer has not been set." );
		return;
	}

	alSourcePlay( m_Source );
}

void OpenALSound::Stop() const
{
	alSourceStop( m_Source );
}

void OpenALSound::Pause() const
{
	alSourcePause( m_Source );
}

void OpenALSound::Resume() const
{
	alSourcePlay( m_Source );
}

void OpenALSound::SetBuffer( const ALuint buffer )
{
	if ( IsPlaying() )
	{
		Stop();
	}

	m_Buffer = buffer;
	alSourcei( m_Source, AL_BUFFER, static_cast<ALint>( m_Buffer ) );
}

void OpenALSound::SetPitch( float pitch )
{
	m_Pitch = pitch;
	alSourcef( m_Source, AL_PITCH, m_Pitch );
}

void OpenALSound::SetGain( float gain )
{
	m_Gain = gain;
	alSourcef( m_Source, AL_GAIN, m_Gain );
}

void OpenALSound::SetPosition( float x, float y, float z )
{
	m_Position[0] = x;
	m_Position[1] = y;
	m_Position[2] = z;
	alSource3f( m_Source, AL_POSITION, x, y, z );
}

void OpenALSound::SetVelocity( float x, float y, float z )
{
	m_Velocity[0] = x;
	m_Velocity[1] = y;
	m_Velocity[2] = z;
	alSource3f( m_Source, AL_VELOCITY, x, y, z );
}

void OpenALSound::SetLooping( bool loop )
{
	m_LoopSound = loop;
	alSourcei( m_Source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE );
}

bool OpenALSound::IsPlaying() const
{
	ALint state;
	alGetSourcei( m_Source, AL_SOURCE_STATE, &state );
	return ( state == AL_PLAYING );
}

bool OpenALSound::IsPaused() const
{
	ALint state;
	alGetSourcei( m_Source, AL_SOURCE_STATE, &state );
	return ( state == AL_PAUSED );
}

bool OpenALSound::IsStopped() const
{
	ALint state;
	alGetSourcei( m_Source, AL_SOURCE_STATE, &state );
	return ( state == AL_STOPPED );
}

bool OpenALSound::IsBufferValid() const
{
	return m_Buffer != 0;
}

bool OpenALSound::IsBufferSame( const ALuint buffer ) const
{
	return m_Buffer == buffer;
}

void OpenALSound::CreateLuaOpenALSoundBind( sol::state& lua )
{
	lua.new_usertype<OpenALSound>(
		"OpenALSound",
		"type_id", &entt::type_hash<OpenALSound>::value,
		sol::call_constructor,
		sol::factories(
			[&]() {
				return OpenALSound();
			} ),

		// Playback control
		"Play", &OpenALSound::Play,
		"Stop", &OpenALSound::Stop,
		"Pause", &OpenALSound::Pause,
		"Resume", &OpenALSound::Resume,

		// Property setters
		"SetPitch", &OpenALSound::SetPitch,
		"SetGain", &OpenALSound::SetGain,
		"SetPosition", &OpenALSound::SetPosition,
		"SetVelocity", &OpenALSound::SetVelocity,
		"SetLooping", &OpenALSound::SetLooping,

		// State queries
		"IsPlaying", &OpenALSound::IsPlaying,
		"IsPaused", &OpenALSound::IsPaused,
		"IsStopped", &OpenALSound::IsStopped );
}
