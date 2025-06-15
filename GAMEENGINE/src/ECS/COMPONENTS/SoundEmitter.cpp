#include "pch.h"

#include "ECS/COMPONENTS/SoundEmitter.h"

#include "ECS/Registry.h"
#include "SYSTEMS/SoundSystem.h"

void SoundEmitter::CreateLuaSoundEmitterBind( sol::state& lua, Registry& registry )
{
	auto& soundSystem = registry.GetContext<std::shared_ptr<SoundSystem>>();
	lua.new_usertype<SoundEmitter>(
		"SoundEmitter",
		"type_id", &entt::type_hash<SoundEmitter>::value,
		sol::call_constructor,
		sol::factories(
			[soundSystem]( std::string eventPath ) {
				return SoundEmitter{
					.eventPath = eventPath,
					.audioEvent = soundSystem->CreateEvent( eventPath ) };
			} ),
		// Data members
		"event_path", &SoundEmitter::eventPath,

		// Audio methods forwarded to AudioEvent
		"set_position", []( SoundEmitter& self, float x, float y ) {
			if (self.audioEvent) self.audioEvent->SetPosition(x, y); },
		"set_volume", []( SoundEmitter& self, float volume ) {
			if (self.audioEvent) self.audioEvent->SetVolume(volume); },
		"get_volume", []( SoundEmitter& self ) -> float { return self.audioEvent ? self.audioEvent->GetVolume() : 0.0f; },
		"play", []( SoundEmitter& self ) {
			if (self.audioEvent) self.audioEvent->Play(); },
		"stop", []( SoundEmitter& self, bool fade ) {
			if (self.audioEvent) self.audioEvent->Stop(fade); },
		"pause", []( SoundEmitter& self, bool pause ) {
			if (self.audioEvent) self.audioEvent->Pause(pause); },
		"is_playing", []( SoundEmitter& self ) -> bool { return self.audioEvent ? self.audioEvent->IsPlaying() : false; },
		"set_parameter", []( SoundEmitter& self, const std::string& name, float value ) {
			if (self.audioEvent) self.audioEvent->SetParameter(name, value); },
		"get_parameter", []( SoundEmitter& self, const std::string& name ) -> float { return self.audioEvent ? self.audioEvent->GetParameter( name ) : 0.0f; } );
}
