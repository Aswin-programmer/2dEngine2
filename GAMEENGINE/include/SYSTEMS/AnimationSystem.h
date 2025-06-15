#pragma once

class Registry;

class AnimationSystem
{
private:
	Registry& m_Registry;

public:
	AnimationSystem( Registry& registry );
	~AnimationSystem() = default;
	void Update();
};
