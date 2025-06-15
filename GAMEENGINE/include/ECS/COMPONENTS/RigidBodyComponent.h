#pragma once

struct RigidBodyComponent
{
	glm::vec2 velocity{ 0.f };

	static void CreateRigidBodyBind( sol::state& lua );
};
