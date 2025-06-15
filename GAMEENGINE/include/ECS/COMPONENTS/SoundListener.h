#pragma once

struct SoundListener
{
	bool isActive = false;
	static void CreateLuaSoundListenerBind( sol::state& lua );
};
