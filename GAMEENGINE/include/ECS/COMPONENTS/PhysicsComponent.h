#pragma once

#include "PHYSICS/Box2DWrappers.h"
#include "PHYSICS/UserData.h"

class Registry;

enum class RigidBodyType
{
	STATIC = 0,
	KINEMATIC,
	DYNAMIC
};

enum class BodyInteractionType
{
	NORMAL = 0,
	COLLIDER = 1,
	SENSOR = 2,
	TRIGGER = 3
};

struct PhysicsAttributes
{
	RigidBodyType eType{ RigidBodyType::STATIC };
	BodyInteractionType bInteractionType{ BodyInteractionType::NORMAL };
	float density{ 1.f }, friction{ 0.2f }, restitution{ 0.2f };
	float restitutionThreshold{ 1.f }, radius{ 0.f }, gravityScale{ 1.f };

	glm::vec2 position{ 0.f }, scale{ 1.f }, boxSize{ 0.f }, offset{ 0.f };
	bool bCircle{ false }, bBoxShape{ true }, bFixedRotation{ true }, bIsSensor{ false }, bIsContactEventsEnabled{ false };

	uint16_t filterCategory{ 0 }, filterMask{ 0 };
	int16_t groupIndex{ 0 };
	ObjectData objectData{};
};

class PhysicsComponent
{
public:
	//// In PhysicsComponent class declaration
	// PhysicsComponent(const PhysicsComponent&) = delete;
	// PhysicsComponent& operator=(const PhysicsComponent&) = delete;

	//// In PhysicsComponent class declaration
	// PhysicsComponent(PhysicsComponent&& other) noexcept;
	// PhysicsComponent& operator=(PhysicsComponent&& other) noexcept;

	PhysicsComponent() = default;
	PhysicsComponent( const b2WorldId worldId, const PhysicsAttributes& physicsAttr );
	~PhysicsComponent() = default;

	void Init( int windowWidth, int windowHeight );
	b2BodyId getBodyID() const { return m_pBody ? m_pBody->bodyId : b2_nullBodyId; }
	glm::vec2 BodyPosition();

	const bool IsSensor() const;
	UserData* GetUserData() { return m_pUserData.get(); }

	PhysicsAttributes& GetAttributes() { return m_InitialAttribs; }

	static void CreatePhysicsLuaBind( sol::state& lua, Registry& registry );

private:
	b2WorldId worldId;
	std::shared_ptr<BodyWrapper> m_pBody;
	b2ShapeId shapeId;

	std::shared_ptr<UserData> m_pUserData;
	PhysicsAttributes m_InitialAttribs;
};
