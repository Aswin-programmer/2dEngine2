#pragma once

class Registry;

class PhysicsSystem
{
private:
	Registry& m_Registry;

public:
	PhysicsSystem( Registry& registry );
	~PhysicsSystem() = default;

	void Update( entt::registry& registry );
};
