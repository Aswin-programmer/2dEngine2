#include "pch.h"

#include "ECS/COMPONENTS/OpenALSoundComponent.h"

#include "ECS/MainRegistry.h"
#include "LOGGER/log.h"
#include "RESOURCES/AssetManager.h"
#include "SOUNDS/ESSENTIALS/OpenALSound.h"

void OpenALSoundComponent::CreateLuaOpenALSoundComponentBind( sol::state& lua )
{
	lua.new_usertype<OpenALSoundComponent>(
		"OpenALSoundComponent",
		"type_id", &entt::type_hash<OpenALSoundComponent>::value,
		sol::call_constructor,
		sol::factories(
			// Make sound component with empty sound
			[] { return OpenALSoundComponent{ .pSound = std::make_shared<OpenALSound>() }; },
			[&]( const std::string& sSoundName ) {
				auto& assetManager = MAIN_REGISTRY().GetAssetManager();
				ALuint bufferToPlay = assetManager.GetSoundEffect( sSoundName );
				OpenALSoundComponent soundComponent{ .pSound = std::make_shared<OpenALSound>() };
				if ( bufferToPlay == 0 )
				{
					LOG_ERROR( "Failed to set OpenAL sound. [{}] is invalid.", sSoundName );
					return soundComponent;
				}

				soundComponent.pSound->SetBuffer( bufferToPlay );
				return soundComponent;
			} ),

		// Playback control
		"Play", sol::overload( []( OpenALSoundComponent& self ) {
			if (!self.pSound->IsBufferValid())
			{
				LOG_ERROR("Failed to play sound. Buffer is not valid.");
				return;
			}

			self.pSound->Play(); }, [&]( OpenALSoundComponent& self, const std::string& sSoundName ) {
			auto& assetManager = MAIN_REGISTRY().GetAssetManager();
			ALuint bufferToPlay = assetManager.GetSoundEffect(sSoundName);
			if (bufferToPlay == 0)
			{
				LOG_ERROR("Failed to play sound. [{}] is invalid.", sSoundName);
				return;
			}

			if (!self.pSound->IsBufferSame(bufferToPlay))
			{
				self.pSound->SetBuffer(bufferToPlay);
			}

			self.pSound->Play(); } ),
		"Stop", []( OpenALSoundComponent& self ) { self.pSound->Stop(); },
		"Pause", []( OpenALSoundComponent& self ) { self.pSound->Pause(); },
		"Resume", []( OpenALSoundComponent& self ) { self.pSound->Resume(); },

		// Property setters
		"SetPitch", []( OpenALSoundComponent& self, float pitch ) { self.pSound->SetPitch( pitch ); },
		"SetGain", []( OpenALSoundComponent& self, float gain ) { self.pSound->SetGain( gain ); },
		"SetPosition", []( OpenALSoundComponent& self, float x, float y, float z ) { self.pSound->SetPosition( x, y, z ); },
		"SetVelocity", []( OpenALSoundComponent& self, float x, float y, float z ) { self.pSound->SetVelocity( x, y, z ); },
		"SetLooping", []( OpenALSoundComponent& self, bool bLooping ) { self.pSound->SetLooping( bLooping ); },

		// State queries
		"IsPlaying", []( OpenALSoundComponent& self ) { return self.pSound->IsPlaying(); },
		"IsPaused", []( OpenALSoundComponent& self ) { return self.pSound->IsPaused(); },
		"IsStopped", []( OpenALSoundComponent& self ) { return self.pSound->IsStopped(); } );
}
