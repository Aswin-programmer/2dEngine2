#pragma once

struct CircleColliderComponent
{
	float radius{ 0.f };
	glm::vec2 offset{ glm::vec2{ 0 } };
	bool bColliding{ false };
	static void CreateLuaCircleColliderBind( sol::state& lua );
};
