#pragma once

struct AnimationComponent
{
	int numFrames{ 1 }, frameRate{ 1 }, frameOffset{ 0 }, currentFrame{ 0 };
	double startTime{ glfwGetTime() };
	bool bVertical{ false }, bLooped{ false };
	static void CreateAnimationLuaBind( sol::state& lua );
};
