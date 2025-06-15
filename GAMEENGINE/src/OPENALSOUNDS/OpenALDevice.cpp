#include "pch.h"

#include "OPENALSOUNDS/OpenALDevice.h"

#include "LOGGER/log.h"

OpenALDevice::OpenALDevice()
{
	p_ALCDevice = alcOpenDevice( nullptr ); // nullptr to get the default device.
	if ( !p_ALCDevice )
	{
		LOG_ERROR( "Failed to get the soud device!" );
		throw( "Failed to get the soud device!" );
	}

	p_ALCContext = alcCreateContext( p_ALCDevice, nullptr ); // Create context
	if ( !p_ALCContext )
	{
		LOG_ERROR( "Failed to set the sound context!" );
		throw( "Failed to set the sound context!" );
	}

	if ( !alcMakeContextCurrent( p_ALCContext ) ) // make current context!
	{
		LOG_ERROR( "Failed to make context current!" );
		throw( "Failed to make context current!" );
	}

	const ALCchar* name = nullptr;
	if ( alcIsExtensionPresent( p_ALCDevice, "ALC_ENUMERATE_ALL_EXT" ) )
	{
		name = alcGetString( p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER );
	}
	if ( !name || alcGetError( p_ALCDevice ) != AL_NO_ERROR )
	{
		name = alcGetString( p_ALCDevice, ALC_DEVICE_SPECIFIER );
	}
	LOG_INFO( "Opened Device : {}", name );
	std::cout << "Opened Device : " << name << std::endl;
}

OpenALDevice::~OpenALDevice()
{
	if ( !alcMakeContextCurrent( nullptr ) )
	{
		LOG_ERROR( "Failed to set the context to nullptr!" );
		throw( "Failed to set the context to nullptr!" );
	}

	alcDestroyContext( p_ALCContext );
	if ( p_ALCContext )
	{
		LOG_ERROR( "Failed to unset the context during close!" );
		throw( "Failed to unset the context during close!" );
	}

	if ( !alcCloseDevice( p_ALCDevice ) )
	{
		LOG_ERROR( "Failed to close sound device!" );
		throw( "Failed to close sound device!" );
	}

	LOG_INFO( "Successfully Disconnected The OpenAL Device!" );
}
