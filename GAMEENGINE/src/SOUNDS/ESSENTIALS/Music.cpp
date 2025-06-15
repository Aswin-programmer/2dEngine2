#include "pch.h"

#include "SOUNDS/ESSENTIALS/Music.h"

#include "LOGGER/log.h"

Music::Music( const SoundParams& params )
	: m_Params{ params }, m_pDecoder{ std::make_unique<ma_decoder>() }
{
}

Music::~Music()
{
	if ( m_IsLoaded )
	{
		ma_decoder_uninit( m_pDecoder.get() );
	}
}

bool Music::Load()
{
	if ( m_IsLoaded )
	{
		return true;
	}

	ma_result result = ma_decoder_init_file( m_Params.filename.c_str(), nullptr, m_pDecoder.get() );
	if ( result != MA_SUCCESS )
	{
		LOG_ERROR( "Failed to load music [{0}] at path [{1}] -- miniaudio error code: {2}",
			m_Params.name, m_Params.filename, static_cast<int>( result ) );
		return false;
	}

	// Get duration if not already set
	if ( m_Params.duration <= 0.0 )
	{
		ma_uint64 frameCount = 0;
		ma_decoder_get_length_in_pcm_frames( m_pDecoder.get(), &frameCount );

		// Use the decoder's config to get the sample rate
		ma_uint32 sampleRate = m_pDecoder->outputSampleRate;

		if ( frameCount > 0 && sampleRate > 0 )
		{
			m_Params.duration = (double)frameCount / (double)sampleRate;
		}
	}

	m_IsLoaded = true;
	return true;
}
