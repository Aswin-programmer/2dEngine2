#include "pch.h"

#include "ECS/COMPONENTS/SoundListener.h"

void SoundListener::CreateLuaSoundListenerBind( sol::state& lua )
{
	lua.new_usertype<SoundListener>(
		"SoundListener",
		"type_id", &entt::type_hash<SoundListener>::value,
		sol::call_constructor,
		sol::factories(
			[]( bool isActive ) {
				if ( isActive )
				{
					return SoundListener{
						.isActive = isActive };
				}
			} ) );
}
