#pragma once

class OpenALDevice
{
public:
	OpenALDevice();
	~OpenALDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;
};
