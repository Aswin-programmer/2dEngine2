#pragma once

struct OpenALSoundComponent
{
	/* OpenAL sound to play. */
	std::shared_ptr<class OpenALSound> pSound{ nullptr };

	static void CreateLuaOpenALSoundComponentBind( sol::state& lua );
};
