#include "pch.h"

#include "SOUND/FMODAudioSystem.h"

#include "LOGGER/log.h"

FMODAudioSystem& FMODAudioSystem::GetInstance()
{
	static FMODAudioSystem instance;
	return instance;
}

bool FMODAudioSystem::Initialize( int maxChannels, int studioFlags, int coreFlags )
{
	if ( m_Initialized )
	{
		LOG_ERROR( "FMOD: Already initialized!" );
		return false;
	}

	// Create the Studio System
	FMOD_RESULT result = FMOD::Studio::System::create( &m_StudioSystem );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to create studio system!" );
		return false;
	}

	// Get the core system
	result = m_StudioSystem->getCoreSystem( &m_CoreSystem );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to get core system!" );
		return false;
	}

	// Initialize the studio system
	result = m_CoreSystem->setSoftwareFormat( 0, FMOD_SPEAKERMODE_STEREO, 0 );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to set software format!" );
		return false;
	}

	result = m_CoreSystem->setDSPBufferSize( 1024, 10 );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to set DSP buffer size!" );
		return false;
	}

	result = m_CoreSystem->setSoftwareChannels( maxChannels );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to set max channels!" );
		return false;
	}

	result = m_StudioSystem->initialize( maxChannels, studioFlags, coreFlags, nullptr );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to initialize studio system!" );
		return false;
	}

	// In FMODAudioSystem::Initialize, ensure 3D audio is enabled
	result = m_CoreSystem->set3DSettings( 1.0f, 1.0f, 1.0f ); // doppler scale, distance factor, rolloff scale
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to set 3D settings!" );
		return false;
	}

	m_Initialized = true;

	LOG_INFO( "FMOD: Successfully initialized." );
	return true;
}

void FMODAudioSystem::Shutdown()
{
	if ( !m_Initialized )
	{
		return;
	}

	// Unload all banks
	for ( auto& bank : m_Banks )
	{
		bank.second->unload();
	}
	m_Banks.clear();

	// Release all sounds
	for ( auto& sound : m_Sounds )
	{
		sound.second->release();
	}
	m_Sounds.clear();

	// Release snapshots
	for ( auto& snapshot : m_ActiveSnapshots )
	{
		snapshot.second->release();
	}
	m_ActiveSnapshots.clear();

	// Clear event descriptions
	m_EventDescriptions.clear();

	// Shutdown studio system
	if ( m_StudioSystem )
	{
		m_StudioSystem->release();
		m_StudioSystem = nullptr;
	}

	m_Initialized = false;
	LOG_INFO( "FMOD: Successfully shutdown." );
}

bool FMODAudioSystem::LoadBank( const std::string& bankName, const std::string& bankPath )
{
	if ( !m_Initialized )
	{
		LOG_ERROR( "FMOD: System not initialized!" );
		return false;
	}

	// Check if bank is already loaded
	if ( m_Banks.find( bankName ) != m_Banks.end() )
	{
		LOG_ERROR( "FMOD: Bank '{}' already loaded.", bankName );
		return true;
	}

	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = m_StudioSystem->loadBankFile( bankPath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to load bank '{}' from path: {}", bankName, bankPath );
		return false;
	}

	// Load bank sample data
	result = bank->loadSampleData();
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to load sample data for bank '{}'", bankName );
		bank->unload();
		return false;
	}

	m_Banks[bankName] = bank;
	LOG_INFO( "FMOD: Successfully loaded bank '{}'", bankName );
	return true;
}

bool FMODAudioSystem::UnloadBank( const std::string& bankName )
{
	if ( !m_Initialized )
	{
		LOG_ERROR( "FMOD: System not initialized!" );
		return false;
	}

	auto it = m_Banks.find( bankName );
	if ( it == m_Banks.end() )
	{
		LOG_ERROR( "FMOD: Bank '{}' not found!", bankName );
		return false;
	}

	FMOD_RESULT result = it->second->unload();
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		LOG_ERROR( "FMOD: Failed to unload bank '{}'", bankName );
		return false;
	}

	m_Banks.erase( it );
	LOG_INFO( "FMOD: Successfully unloaded bank '{}'", bankName );
	return true;
}

FMOD::Studio::EventInstance* FMODAudioSystem::CreateEventInstance( const std::string& eventPath )
{
	if ( !m_Initialized )
	{
		LOG_ERROR( "FMOD: System not initialized!" );
		return nullptr;
	}

	// Check if we already have a cached event description
	FMOD::Studio::EventDescription* eventDescription = nullptr;
	auto descIt = m_EventDescriptions.find( eventPath );

	if ( descIt != m_EventDescriptions.end() )
	{
		eventDescription = descIt->second;
	}
	else
	{
		// Get the event description
		FMOD_RESULT result = m_StudioSystem->getEvent( eventPath.c_str(), &eventDescription );
		if ( ErrorCheck( result ) != FMOD_OK || eventDescription == nullptr )
		{
			LOG_ERROR( "FMOD: Failed to get event description for '{}'", eventPath );
			return nullptr;
		}

		// Cache it for future use
		m_EventDescriptions[eventPath] = eventDescription;
	}

	// Create the event instance
	FMOD::Studio::EventInstance* eventInstance = nullptr;
	FMOD_RESULT result = eventDescription->createInstance( &eventInstance );
	if ( ErrorCheck( result ) != FMOD_OK || eventInstance == nullptr )
	{
		LOG_ERROR( "FMOD: Failed to create event instance for '{}'", eventPath );
		return nullptr;
	}

	return eventInstance;
}

bool FMODAudioSystem::ReleaseEvent( FMOD::Studio::EventInstance* eventInstance )
{
	if ( !m_Initialized || eventInstance == nullptr )
	{
		return false;
	}

	FMOD_RESULT result = eventInstance->release();
	return ( ErrorCheck( result ) == FMOD_OK );
}

FMOD::Sound* FMODAudioSystem::LoadSound( const std::string& soundPath, bool loop )
{
	if ( !m_Initialized )
	{
		LOG_ERROR( "FMOD: System not initialized!" );
		return nullptr;
	}

	// Check if sound is already loaded
	auto it = m_Sounds.find( soundPath );
	if ( it != m_Sounds.end() )
	{
		return it->second;
	}

	FMOD::Sound* sound = nullptr;
	FMOD_MODE mode = FMOD_DEFAULT;
	if ( loop )
	{
		mode |= FMOD_LOOP_NORMAL;
	}

	FMOD_RESULT result = m_CoreSystem->createSound( soundPath.c_str(), mode, nullptr, &sound );
	if ( ErrorCheck( result ) != FMOD_OK || sound == nullptr )
	{
		LOG_ERROR( "FMOD: Failed to load sound '{}'", soundPath );
		return nullptr;
	}

	m_Sounds[soundPath] = sound;
	return sound;
}

FMOD::Channel* FMODAudioSystem::PlaySound( FMOD::Sound* sound, float volume )
{
	if ( !m_Initialized || sound == nullptr )
	{
		return nullptr;
	}

	FMOD::Channel* channel = nullptr;
	FMOD_RESULT result = m_CoreSystem->playSound( sound, nullptr, false, &channel );
	if ( ErrorCheck( result ) != FMOD_OK || channel == nullptr )
	{
		return nullptr;
	}

	channel->setVolume( volume );
	return channel;
}

bool FMODAudioSystem::PlayOneShot( const std::string& eventPath, const FMOD_VECTOR& position )
{
	if ( !m_Initialized )
	{
		return false;
	}

	FMOD::Studio::EventInstance* eventInstance = CreateEventInstance( eventPath );
	if ( eventInstance == nullptr )
	{
		return false;
	}

	// Create a fully initialized FMOD_3D_ATTRIBUTES structure
	FMOD_3D_ATTRIBUTES attributes = {};
	attributes.position = position;
	attributes.velocity = { 0.0f, 0.0f, 0.0f }; // Initialize velocity
	attributes.forward = { 0.0f, 0.0f, 1.0f };	// Forward vector (z-axis)
	attributes.up = { 0.0f, 1.0f, 0.0f };		// Up vector (y-axis)

	// Set the 3D attributes
	FMOD_RESULT result = eventInstance->set3DAttributes( &attributes );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		eventInstance->release();
		return false;
	}

	// Start the event
	result = eventInstance->start();
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		eventInstance->release();
		return false;
	}

	// Set to automatically release when done
	result = eventInstance->release();
	return ( ErrorCheck( result ) == FMOD_OK );
}

void FMODAudioSystem::Set3DListenerPosition( float x, float y )
{
	if ( !m_Initialized )
		return;

	FMOD_3D_ATTRIBUTES listenerAttributes;
	listenerAttributes.position = { x, y, 0 };
	listenerAttributes.velocity = { 0, 0, 0 };
	listenerAttributes.forward = { 0, 0, 1 }; // Forward direction (z-axis)
	listenerAttributes.up = { 0, 1, 0 };	  // Up direction (y-axis)

	// Update Studio System listener (index 0)
	FMOD_RESULT result = m_StudioSystem->setListenerAttributes( 0, &listenerAttributes );
	ErrorCheck( result );

	//// Optional: Verify the position via Studio System
	// FMOD_3D_ATTRIBUTES verifyAttrs;
	// result = m_StudioSystem->getListenerAttributes(0, &verifyAttrs);
	// if (result == FMOD_OK) {
	//     std::cout << "Studio Listener Position: " << verifyAttrs.position.x
	//         << ", " << verifyAttrs.position.y << std::endl;
	// }
}

void FMODAudioSystem::Set3DEventPosition( FMOD::Studio::EventInstance* eventInstance, float x, float y )
{
	if ( !m_Initialized || eventInstance == nullptr )
	{
		return;
	}

	FMOD_3D_ATTRIBUTES attributes;
	FMOD_VECTOR position = { x, y, 0 };
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
	// Use default forward and up vectors for the event source.
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };

	attributes.position = position;
	attributes.velocity = velocity;
	attributes.forward = forward;
	attributes.up = up;

	FMOD_RESULT result = eventInstance->set3DAttributes( &attributes );

	//// Verify position was set by reading it back
	// FMOD_3D_ATTRIBUTES verifyAttributes;
	// result = eventInstance->get3DAttributes(&verifyAttributes);
	// if (result == FMOD_OK) {
	//     std::cout << "Verified position: " << verifyAttributes.position.x
	//         << ", " << verifyAttributes.position.y << std::endl;
	// }

	ErrorCheck( result );
}

bool FMODAudioSystem::SetEventParameter( FMOD::Studio::EventInstance* eventInstance, const std::string& parameterName, float value )
{
	if ( !m_Initialized || eventInstance == nullptr )
	{
		return false;
	}

	FMOD_RESULT result = eventInstance->setParameterByName( parameterName.c_str(), value );
	return ( ErrorCheck( result ) == FMOD_OK );
}

float FMODAudioSystem::GetEventParameter( FMOD::Studio::EventInstance* eventInstance, const std::string& parameterName )
{
	if ( !m_Initialized || eventInstance == nullptr )
	{
		return 0.0f;
	}

	float value = 0.0f;
	FMOD_RESULT result = eventInstance->getParameterByName( parameterName.c_str(), &value );
	ErrorCheck( result );

	return value;
}

bool FMODAudioSystem::SetGlobalParameter( const std::string& parameterName, float value )
{
	if ( !m_Initialized )
	{
		return false;
	}

	FMOD_RESULT result = m_StudioSystem->setParameterByName( parameterName.c_str(), value );
	return ( ErrorCheck( result ) == FMOD_OK );
}

float FMODAudioSystem::GetGlobalParameter( const std::string& parameterName )
{
	if ( !m_Initialized )
	{
		return 0.0f;
	}

	float value = 0.0f;
	FMOD_RESULT result = m_StudioSystem->getParameterByName( parameterName.c_str(), &value );
	ErrorCheck( result );

	return value;
}

FMOD::Studio::Bus* FMODAudioSystem::GetBus( const std::string& busPath )
{
	if ( !m_Initialized )
	{
		return nullptr;
	}

	FMOD::Studio::Bus* bus = nullptr;
	FMOD_RESULT result = m_StudioSystem->getBus( busPath.c_str(), &bus );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		return nullptr;
	}

	return bus;
}

bool FMODAudioSystem::SetBusVolume( const std::string& busPath, float volume )
{
	FMOD::Studio::Bus* bus = GetBus( busPath );
	if ( bus == nullptr )
	{
		return false;
	}

	FMOD_RESULT result = bus->setVolume( volume );
	return ( ErrorCheck( result ) == FMOD_OK );
}

float FMODAudioSystem::GetBusVolume( const std::string& busPath )
{
	float volume = 0.0f;
	FMOD::Studio::Bus* bus = GetBus( busPath );
	if ( bus == nullptr )
	{
		return volume;
	}

	FMOD_RESULT result = bus->getVolume( &volume );
	ErrorCheck( result );

	return volume;
}

bool FMODAudioSystem::SetBusPaused( const std::string& busPath, bool paused )
{
	FMOD::Studio::Bus* bus = GetBus( busPath );
	if ( bus == nullptr )
	{
		return false;
	}

	FMOD_RESULT result = bus->setPaused( paused );
	return ( ErrorCheck( result ) == FMOD_OK );
}

bool FMODAudioSystem::GetBusPaused( const std::string& busPath )
{
	bool paused = false;
	FMOD::Studio::Bus* bus = GetBus( busPath );
	if ( bus == nullptr )
	{
		return paused;
	}

	FMOD_RESULT result = bus->getPaused( &paused );
	ErrorCheck( result );

	return paused;
}

FMOD::Studio::VCA* FMODAudioSystem::GetVCA( const std::string& vcaPath )
{
	if ( !m_Initialized )
	{
		return nullptr;
	}

	FMOD::Studio::VCA* vca = nullptr;
	FMOD_RESULT result = m_StudioSystem->getVCA( vcaPath.c_str(), &vca );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		return nullptr;
	}

	return vca;
}

bool FMODAudioSystem::SetVCAVolume( const std::string& vcaPath, float volume )
{
	FMOD::Studio::VCA* vca = GetVCA( vcaPath );
	if ( vca == nullptr )
	{
		return false;
	}

	FMOD_RESULT result = vca->setVolume( volume );
	return ( ErrorCheck( result ) == FMOD_OK );
}

float FMODAudioSystem::GetVCAVolume( const std::string& vcaPath )
{
	float volume = 0.0f;
	FMOD::Studio::VCA* vca = GetVCA( vcaPath );
	if ( vca == nullptr )
	{
		return volume;
	}

	FMOD_RESULT result = vca->getVolume( &volume );
	ErrorCheck( result );

	return volume;
}

bool FMODAudioSystem::StartSnapshot( const std::string& snapshotPath )
{
	if ( !m_Initialized )
	{
		return false;
	}

	// Check if snapshot is already active
	auto it = m_ActiveSnapshots.find( snapshotPath );
	if ( it != m_ActiveSnapshots.end() )
	{
		// Already active
		return true;
	}

	// Create the snapshot event instance
	FMOD::Studio::EventInstance* snapshotInstance = CreateEventInstance( snapshotPath );
	if ( snapshotInstance == nullptr )
	{
		return false;
	}

	// Start the snapshot
	FMOD_RESULT result = snapshotInstance->start();
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		snapshotInstance->release();
		return false;
	}

	m_ActiveSnapshots[snapshotPath] = snapshotInstance;
	return true;
}

bool FMODAudioSystem::StopSnapshot( const std::string& snapshotPath )
{
	if ( !m_Initialized )
	{
		return false;
	}

	auto it = m_ActiveSnapshots.find( snapshotPath );
	if ( it == m_ActiveSnapshots.end() )
	{
		// Not active
		return true;
	}

	// Stop the snapshot
	FMOD_RESULT result = it->second->stop( FMOD_STUDIO_STOP_ALLOWFADEOUT );
	if ( ErrorCheck( result ) != FMOD_OK )
	{
		return false;
	}

	// Release the instance
	it->second->release();
	m_ActiveSnapshots.erase( it );

	return true;
}

void FMODAudioSystem::Update()
{
	if ( !m_Initialized )
	{
		return;
	}

	m_StudioSystem->update();
}

FMOD_RESULT FMODAudioSystem::ErrorCheck( FMOD_RESULT result ) const
{
	if ( result != FMOD_OK )
	{
		LOG_ERROR( "FMOD Error: {} - {}", static_cast<int>( result ), FMOD_ErrorString( result ) );
	}

	return result;
}
