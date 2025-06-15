#include "pch.h"

#include "SYSTEMS/SoundSystem.h"

#include "ECS/Registry.h"
#include "ECS/COMPONENTS/SoundEmitter.h"
#include "ECS/COMPONENTS/SoundListener.h"
#include "ECS/COMPONENTS/TransformComponent.h"
#include "SOUND/AudioEvent.h"
#include "SOUND/FMODAudioSystem.h"

bool SoundSystem::Initialize()
{
	return FMODAudioSystem::GetInstance().Initialize();
}

void SoundSystem::Shutdown()
{
	// Stop and clear all active events
	m_ActiveEvents.clear();
	m_CurrentMusicTrack = nullptr;

	// Shutdown FMOD
	FMODAudioSystem::GetInstance().Shutdown();
}

bool SoundSystem::LoadBanks( const std::string& banksFolder )
{
	try
	{
		for ( const auto& entry : std::filesystem::directory_iterator( banksFolder ) )
		{
			if ( entry.is_regular_file() )
			{
				std::string filename = entry.path().filename().string();
				std::string extension = entry.path().extension().string();

				// Only load .bank files
				if ( extension == ".bank" )
				{
					std::string bankName = entry.path().stem().string();
					std::string bankPath = entry.path().string();

					if ( !LoadBank( bankName, bankPath ) )
					{
						std::cerr << "Failed to load bank: " << bankName << std::endl;
					}
				}
			}
		}
		return true;
	}
	catch ( const std::exception& e )
	{
		std::cerr << "Error loading banks: " << e.what() << std::endl;
		return false;
	}
}

bool SoundSystem::LoadBank( const std::string& bankName, const std::string& bankPath )
{
	return FMODAudioSystem::GetInstance().LoadBank( bankName, bankPath );
}

std::shared_ptr<AudioEvent> SoundSystem::CreateEvent( const std::string& eventPath )
{
	auto event = std::make_shared<AudioEvent>( eventPath );
	if ( event->IsValid() )
	{
		m_ActiveEvents.push_back( event );
		return event;
	}

	return nullptr;
}

bool SoundSystem::PlayOneShot( const std::string& eventPath, float x, float y )
{
	FMOD_VECTOR position = { x / 32, -( y / 32 ), 0 };
	return FMODAudioSystem::GetInstance().PlayOneShot( eventPath, position );
}

void SoundSystem::SetListenerPosition( float x, float y )
{
	FMODAudioSystem::GetInstance().Set3DListenerPosition( x, y );
}

bool SoundSystem::SetGlobalParameter( const std::string& name, float value )
{
	return FMODAudioSystem::GetInstance().SetGlobalParameter( name, value );
}

float SoundSystem::GetGlobalParameter( const std::string& name )
{
	return FMODAudioSystem::GetInstance().GetGlobalParameter( name );
}

bool SoundSystem::SetBusVolume( const std::string& busPath, float volume )
{
	return FMODAudioSystem::GetInstance().SetBusVolume( busPath, volume );
}

bool SoundSystem::SetVCAVolume( const std::string& vcaPath, float volume )
{
	return FMODAudioSystem::GetInstance().SetVCAVolume( vcaPath, volume );
}

bool SoundSystem::StartSnapshot( const std::string& snapshotPath )
{
	return FMODAudioSystem::GetInstance().StartSnapshot( snapshotPath );
}

bool SoundSystem::StopSnapshot( const std::string& snapshotPath )
{
	return FMODAudioSystem::GetInstance().StopSnapshot( snapshotPath );
}

std::shared_ptr<AudioEvent> SoundSystem::PlayMusicTrack( const std::string& musicEventPath )
{
	// Stop current music first
	StopAllMusic();

	// Create and play the new music track
	m_CurrentMusicTrack = CreateEvent( musicEventPath );
	if ( m_CurrentMusicTrack )
	{
		m_CurrentMusicTrack->Play();
	}

	return m_CurrentMusicTrack;
}

bool SoundSystem::StopAllMusic( bool allowFadeOut )
{
	if ( m_CurrentMusicTrack )
	{
		bool result = m_CurrentMusicTrack->Stop( allowFadeOut );
		m_CurrentMusicTrack = nullptr;
		return result;
	}

	return true;
}

void SoundSystem::Update( float deltaTime, Registry& registry )
{
	// UpdateListenerPosition
	UpdateListenerPosition( registry );
	// UpdateEntityPosition
	UpdateEntityPosition( registry );

	// Accumulate delta time
	m_TimeSinceLastUpdate += deltaTime;

	// Update FMOD at fixed intervals (e.g., 60Hz)
	const float updateInterval = 1.0f / 60.0f;
	if ( m_TimeSinceLastUpdate >= updateInterval )
	{
		// Update FMOD
		FMODAudioSystem::GetInstance().Update();

		// Clean up finished events
		CleanupEvents();

		// Reset timer
		m_TimeSinceLastUpdate = 0.0f;
	}
}

void SoundSystem::UpdateEntityPosition( Registry& registry )
{
	// Update entity-based sounds
	auto view = registry.GetRegistry().view<TransformComponent, SoundEmitter>();
	for ( auto entity : view )
	{
		auto& transform = view.get<TransformComponent>( entity );
		auto& emitter = view.get<SoundEmitter>( entity );

		emitter.audioEvent->SetPosition(
			transform.position.x / 32,
			-( transform.position.y / 32 ) );
	}
}

void SoundSystem::UpdateListenerPosition( Registry& registry )
{
	// Update Listener Position
	auto view = registry.GetRegistry().view<TransformComponent, SoundListener>();
	for ( auto entity : view )
	{
		auto& transform = view.get<TransformComponent>( entity );
		auto& listener = view.get<SoundListener>( entity );

		SetListenerPosition(
			transform.position.x / 32,
			-( transform.position.y / 32 ) );
	}
}

void SoundSystem::CreateLuaSoundSystemBind( sol::state& lua, Registry& registry )
{
	lua.new_usertype<SoundSystem>(
		"SoundSystem",
		sol::no_constructor, // Prevents direct instantiation in Lua

		// Bank loading
		"load_banks", &SoundSystem::LoadBanks,
		"load_bank", &SoundSystem::LoadBank,
		"play_one_shot", &SoundSystem::PlayOneShot,

		// Global parameters
		"set_global_parameter", &SoundSystem::SetGlobalParameter,
		"get_global_parameter", &SoundSystem::GetGlobalParameter,

		// Mixing control
		"set_bus_volume", &SoundSystem::SetBusVolume,
		"set_vca_volume", &SoundSystem::SetVCAVolume,

		// Snapshot control
		"start_snapshot", &SoundSystem::StartSnapshot,
		"stop_snapshot", &SoundSystem::StopSnapshot,

		// Music control
		"play_music_track", &SoundSystem::PlayMusicTrack,
		"stop_all_music", &SoundSystem::StopAllMusic );
	auto soundSystem = registry.GetContext<std::shared_ptr<SoundSystem>>();
	lua["SoundSystem"] = *soundSystem;
}

void SoundSystem::CleanupEvents()
{
	// Remove any events that are no longer active or valid
	m_ActiveEvents.erase(
		std::remove_if( m_ActiveEvents.begin(), m_ActiveEvents.end(),
			[]( const std::shared_ptr<AudioEvent>& event ) {
				return !event->IsValid() || !event->IsPlaying();
			} ),
		m_ActiveEvents.end() );

	// Check if music track is still playing
	if ( m_CurrentMusicTrack && ( !m_CurrentMusicTrack->IsValid() || !m_CurrentMusicTrack->IsPlaying() ) )
	{
		m_CurrentMusicTrack = nullptr;
	}
}
