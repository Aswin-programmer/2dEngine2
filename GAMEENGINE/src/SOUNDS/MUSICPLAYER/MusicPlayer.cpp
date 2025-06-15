#include "pch.h"

#include "SOUNDS/ESSENTIALS/Music.h"
#include "SOUNDS/MUSICPLAYER/MusicPlayer.h"

#include "LOGGER/log.h"

void MusicPlayer::DataCallback( ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount )
{
	MusicPlayer* player = (MusicPlayer*)pDevice->pUserData;
	if ( player )
	{
		player->ProcessAudio( pOutput, frameCount );
	}
}

MusicPlayer::MusicPlayer()
	: m_pCurrentDecoder( nullptr )
{
	m_DeviceConfig = ma_device_config_init( ma_device_type_playback );
	m_DeviceConfig.playback.format = ma_format_f32;
	m_DeviceConfig.playback.channels = 2;
	m_DeviceConfig.sampleRate = 44100;
	m_DeviceConfig.dataCallback = DataCallback;
	m_DeviceConfig.pUserData = this;

	ma_result result = ma_device_init( nullptr, &m_DeviceConfig, &m_Device );
	if ( result != MA_SUCCESS )
	{
		LOG_ERROR( "Failed to initialize miniaudio device - error code: {0}", static_cast<int>( result ) );
		return;
	}

	LOG_INFO( "MusicPlayer initialized successfully" );
}

MusicPlayer::~MusicPlayer()
{
	Stop();
	ma_device_uninit( &m_Device );
	LOG_INFO( "Music Player Closed!" );
}

void MusicPlayer::Play( Music& music, int loops )
{
	Stop();

	if ( !music.IsLoaded() && !music.Load() )
	{
		LOG_ERROR( "Failed to play music [{0}] - Music not loaded!", music.GetName() );
		return;
	}

	m_pCurrentDecoder = music.GetDecoder();
	if ( !m_pCurrentDecoder )
	{
		LOG_ERROR( "Failed to play music [{0}] - Decoder was Null!", music.GetName() );
		return;
	}

	// Reset the decoder position to the start
	ma_decoder_seek_to_pcm_frame( m_pCurrentDecoder, 0 );

	m_LoopCount = loops;
	m_CurrentLoop = 0;
	m_IsPlaying = true;
	m_IsPaused = false;

	ma_result result = ma_device_start( &m_Device );
	if ( result != MA_SUCCESS )
	{
		LOG_ERROR( "Failed to start playback for [{0}] - miniaudio error code: {1}",
			music.GetName(), static_cast<int>( result ) );
		m_IsPlaying = false;
		m_pCurrentDecoder = nullptr;
	}
}

void MusicPlayer::Pause()
{
	if ( m_IsPlaying && !m_IsPaused )
	{
		ma_device_stop( &m_Device );
		m_IsPaused = true;
	}
}

void MusicPlayer::Resume()
{
	if ( m_IsPlaying && m_IsPaused )
	{
		ma_device_start( &m_Device );
		m_IsPaused = false;
	}
}

void MusicPlayer::Stop()
{
	if ( m_IsPlaying )
	{
		ma_device_stop( &m_Device );
		m_IsPlaying = false;
		m_IsPaused = false;
		m_pCurrentDecoder = nullptr;
	}
}

void MusicPlayer::SetVolume( int volume )
{
	m_Volume = ( volume < 0 ) ? 0 : ( ( volume > 100 ) ? 100 : volume );
	float volumeScale = (float)m_Volume / 100.0f;
	ma_device_set_master_volume( &m_Device, volumeScale );
}

bool MusicPlayer::IsPlaying()
{
	return m_IsPlaying && !m_IsPaused;
}

void MusicPlayer::ProcessAudio( void* pOutput, ma_uint32 frameCount )
{
	if ( !m_pCurrentDecoder || !m_IsPlaying )
	{
		// Clear buffer with zeros instead of using ma_silence
		size_t bytesPerFrame = m_DeviceConfig.playback.channels * ma_get_bytes_per_sample( m_DeviceConfig.playback.format );
		size_t totalBytes = frameCount * bytesPerFrame;
		memset( pOutput, 0, totalBytes );
		return;
	}

	// Read directly from the decoder into the output buffer
	ma_uint64 framesRead = 0;
	ma_decoder_read_pcm_frames( m_pCurrentDecoder, pOutput, frameCount, &framesRead );

	// If we reached the end of the file
	if ( framesRead < frameCount )
	{
		// Clear the rest of the buffer with zeros
		size_t bytesPerFrame = m_DeviceConfig.playback.channels * ma_get_bytes_per_sample( m_DeviceConfig.playback.format );
		size_t bytesRead = (size_t)framesRead * bytesPerFrame;
		size_t bytesRemaining = (size_t)( frameCount - framesRead ) * bytesPerFrame;

		uint8_t* pBufferOffset = (uint8_t*)pOutput + bytesRead;
		memset( pBufferOffset, 0, bytesRemaining );

		// Handle looping
		if ( m_LoopCount == -1 || ( m_LoopCount > 0 && m_CurrentLoop < m_LoopCount ) )
		{
			ma_decoder_seek_to_pcm_frame( m_pCurrentDecoder, 0 );
			m_CurrentLoop++;
		}
		else
		{
			// Stop when all loops are complete
			m_IsPlaying = false;
			m_pCurrentDecoder = nullptr;
			ma_device_stop( &m_Device );
		}
	}
}
