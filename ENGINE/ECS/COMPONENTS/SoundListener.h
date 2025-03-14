#pragma once

#include "../../SOUNDS/AudioEvent.h"
#include "../../SYSTEMS/SoundSystem.h"
#include "../Registry.h"
#include <sol/sol.hpp>
#include <entt/entt.hpp>

struct SoundListener
{
	bool isActive = false;
	static void CreateLuaSoundListenerBind(sol::state& lua);
};

