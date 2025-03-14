#pragma once

#include "../../SOUNDS/AudioEvent.h"
#include "../../SYSTEMS/SoundSystem.h"
#include "../Registry.h"
#include <sol/sol.hpp>
#include <entt/entt.hpp>

struct SoundEmitter
{
	std::string eventPath;
	std::shared_ptr<AudioEvent> audioEvent;

	static void CreateLuaSoundEmitterBind(sol::state& lua, Registry& registry);
};
