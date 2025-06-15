#pragma once

class OpenALSound
{
public:
	OpenALSound();
	OpenALSound( const ALuint buffer );
	~OpenALSound();

	void Play() const;
	void Stop() const;
	void Pause() const;
	void Resume() const;

	void SetBuffer( const ALuint buffer );
	void SetPitch( float pitch );
	void SetGain( float gain );
	void SetPosition( float x, float y, float z );
	void SetVelocity( float x, float y, float z );
	void SetLooping( bool loop );

	bool IsPlaying() const;
	bool IsPaused() const;
	bool IsStopped() const;
	bool IsBufferValid() const;
	bool IsBufferSame( const ALuint buffer ) const;

	static void CreateLuaOpenALSoundBind( sol::state& lua );

private:
	ALuint m_Source;
	ALuint m_Buffer;
	float m_Pitch;
	float m_Gain;
	float m_Position[3];
	float m_Velocity[3];
	bool m_LoopSound;
};
