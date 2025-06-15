#pragma once

class Music;

class MusicPlayer
{
private:
	ma_device m_Device;
	ma_device_config m_DeviceConfig;
	ma_decoder* m_pCurrentDecoder{ nullptr };
	bool m_IsPlaying{ false };
	bool m_IsPaused{ false };
	int m_Volume{ 100 }; // 0-100 range
	int m_LoopCount{ 0 };
	int m_CurrentLoop{ 0 };

	static void DataCallback( ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount );

public:
	MusicPlayer();
	~MusicPlayer();

	void Play( Music& music, int loops = 0 );
	void Pause();
	void Resume();
	void Stop();
	void SetVolume( int volume ); // 0-100 range
	bool IsPlaying();

	// Called by the callback to get more data
	void ProcessAudio( void* pOutput, ma_uint32 frameCount );
};
