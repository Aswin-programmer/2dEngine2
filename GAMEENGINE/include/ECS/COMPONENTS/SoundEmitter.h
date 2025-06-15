#pragma once

#include "SOUND/AudioEvent.h"

class Registry;

struct SoundEmitter
{
	std::string eventPath;
	std::shared_ptr<AudioEvent> audioEvent;

	static void CreateLuaSoundEmitterBind( sol::state& lua, Registry& registry );
};
