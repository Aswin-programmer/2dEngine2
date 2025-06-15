#pragma once

#include "CoreEngineData.h"
#include "ECS/Entity.h"

class Camera2D;
class Registry;

struct FollowCamParams
{
	float minX{ 0.f }, minY{ 0.f }, scale{ 1.f }, springback{ 1.f };
	float maxX{ static_cast<float>( CoreEngineData::GetInstance().WindowWidth() ) };
	float maxY{ static_cast<float>( CoreEngineData::GetInstance().WindowHeight() ) };
};

class FollowCamera
{
private:
	Camera2D& m_Camera;
	FollowCamParams m_Params;
	Entity m_Entity;

public:
	FollowCamera( Camera2D& camera, const Entity& entity, const FollowCamParams& params );
	~FollowCamera() = default;

	void Update();
	inline void SetEntity( const Entity& entity ) { m_Entity = entity; }
	void SetSpringback( float springback );
	void SetCameraParameters( const FollowCamParams& params );
	inline const FollowCamParams& GetParams() const { return m_Params; }

	static void CreateLuaFollowCamera( sol::state& lua, Registry& registry );
};
