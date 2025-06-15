#pragma once

class FrameBuffer;

enum class FramebufferType
{
	TILEMAP,
	SCENE,
	NO_TYPE
};

struct EditorFramebuffers
{
	std::map<FramebufferType, std::shared_ptr<FrameBuffer>> mapFramebuffers;
};
