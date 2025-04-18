#include "PhysicsComponent.h"
#include "PhysicsComponent.h"
#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(const b2WorldId worldID, const PhysicsAttributes& physicsAttr)
	:
	worldId(worldID),
	m_InitialAttribs(physicsAttr),
	bodyId(b2_nullBodyId),
	shapeId(b2_nullShapeId)
{
}

void PhysicsComponent::Init(int windowWidth, int windowHeight)
{
	if (!b2World_IsValid(worldId))
	{
		LOG_ERROR("Failed to create the Box2D world [{0}]", worldId.index1);
	}

	bool bCircle{ m_InitialAttribs.bCircle };

	//Create the body def
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = static_cast<b2BodyType>(m_InitialAttribs.eType);
	auto PIXELS_TO_METERS = CoreEngineData::GetInstance().PixelsToMeters();
	bodyDef.position = {
		(m_InitialAttribs.position.x + m_InitialAttribs.offset.x - (windowWidth * 0.5f) +
			((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.x) *
				m_InitialAttribs.scale.x) * 0.5f) * PIXELS_TO_METERS
		,
			(m_InitialAttribs.position.y + m_InitialAttribs.offset.y - (windowHeight * 0.5f) +
				((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.y) *
					m_InitialAttribs.scale.y) * 0.5f) * PIXELS_TO_METERS
	};
	bodyDef.gravityScale = m_InitialAttribs.gravityScale;
	bodyDef.fixedRotation = m_InitialAttribs.bFixedRotation;

	//Create the Ridgid Body:
	bodyId = b2CreateBody(worldId, &bodyDef);
	if (!b2Body_IsValid(bodyId))
	{
		LOG_ERROR("Failed to create the Box2D body [{0}]", bodyId.index1);
	}

	//Create the shape:
	b2Circle circleShape{};
	b2Polygon polyShape{};

	if (m_InitialAttribs.bCircle)
	{
		circleShape.radius = m_InitialAttribs.radius * m_InitialAttribs.scale.x;
		circleShape.center = { 0.0f, 0.0f };
	}
	else if (m_InitialAttribs.bBoxShape)
	{
		polyShape = b2MakeBox(
			PIXELS_TO_METERS * m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f,
			PIXELS_TO_METERS * m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f
		);
	}
	else
	{
		// TODO: Create your polygon shape
	}

	//Create the fixture def
	b2ShapeDef fixtureDef = b2DefaultShapeDef();
	fixtureDef.density = m_InitialAttribs.density;
	fixtureDef.friction = m_InitialAttribs.friction;
	fixtureDef.restitution = m_InitialAttribs.restitution;

	if (m_InitialAttribs.bCircle)
	{
		shapeId = b2CreateCircleShape(bodyId, &fixtureDef, &circleShape);
	}
	else
	{
		shapeId = b2CreatePolygonShape(bodyId, &fixtureDef, &polyShape);
	}

	if (!b2Shape_IsValid(shapeId))
	{
		LOG_ERROR("Failed to create the Box2D shape [{0}]", shapeId.index1);
	}
	
}

glm::vec2 PhysicsComponent::BodyPosition()
{
	if (!b2Body_IsValid(bodyId)) return { 0,0 };
	const b2Vec2 pos = b2Body_GetPosition(bodyId);
	auto METERS_TO_PIXELS = CoreEngineData::GetInstance().MetersToPixels();
	return { pos.x * METERS_TO_PIXELS, pos.y * METERS_TO_PIXELS };
}

void PhysicsComponent::CreatePhysicsLuaBind(sol::state& lua, Registry& registry)
{
	lua.new_enum<RigidBodyType>(
		"BodyType", {
			{"Static", RigidBodyType::STATIC },
			{"Kinematic", RigidBodyType::KINEMATIC},
			{"Dynamic", RigidBodyType::DYNAMIC }
		}
	);
	
	lua.new_usertype<PhysicsAttributes>(
		"PhysicsAttributes",
		sol::call_constructor,
		sol::factories(
			[] {
				return PhysicsAttributes{};
			}
			// TODO: Add more specific ctor
				),
		"eType", &PhysicsAttributes::eType,
		"density", &PhysicsAttributes::density,
		"friction", &PhysicsAttributes::friction,
		"restitution", &PhysicsAttributes::restitution,
		"restitutionThreshold", &PhysicsAttributes::restitutionThreshold,
		"radius", &PhysicsAttributes::radius,
		"gravityScale", &PhysicsAttributes::gravityScale,
		"position", &PhysicsAttributes::position,
		"scale", &PhysicsAttributes::scale,
		"boxSize", &PhysicsAttributes::boxSize,
		"offset", &PhysicsAttributes::offset,
		"bCircle", &PhysicsAttributes::bCircle,
		"bBoxShape", &PhysicsAttributes::bBoxShape,
		"bFixedRotation", &PhysicsAttributes::bFixedRotation
		// TODO: Add in filters and other properties as needed
	);

	auto& box2DWrapper = registry.GetContext<std::shared_ptr<Box2DWrappers>>();
	if (!box2DWrapper)
	{
		LOG_ERROR("Failed to get box2DWrapper in PhysicsComponent.cpp");
	}

	lua.new_usertype<PhysicsComponent>(
		"PhysicsComp",
		"type_id", &entt::type_hash<PhysicsComponent>::value,
		sol::call_constructor,
		sol::factories(
			[&](const PhysicsAttributes& attr) {
				PhysicsComponent pc{ box2DWrapper->GetWorldID(), attr };
				pc.Init(640, 480); // TODO: Change based on window values
				return pc;
			}
		),
		"linear_impulse", [](PhysicsComponent& pc, const glm::vec2& impulse) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				LOG_ERROR("Failed to bind the Box2D linear_impulse with lua [{0}]", body.index1);
				return;
			}
			// Get the position in meters directly from Box2D
			b2Vec2 center = b2Body_GetPosition(body);
			b2Body_ApplyLinearImpulse(body, b2Vec2{ impulse.x, impulse.y }, center, true);
		},
		"angular_impulse", [](PhysicsComponent& pc, float impulse) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D angular_impulse with lua [{0}]", body.index1);
				return;
			}

			b2Body_ApplyAngularImpulse(body, impulse, true);
		},
		"set_linear_velocity", [](PhysicsComponent& pc, const glm::vec2& velocity) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D set_linear_velocity with lua [{0}]", body.index1);
				return;
			}

			b2Body_SetLinearVelocity(body, b2Vec2{ velocity.x, velocity.y });
		},
		"get_linear_velocity", [](PhysicsComponent& pc) -> glm::vec2 {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D get_linear_velocity with lua [{0}]", body.index1);
				return glm::vec2(0.0f);
			}

			const auto& linearVelocity = b2Body_GetLinearVelocity(body);
			return glm::vec2{ linearVelocity.x, linearVelocity.y };
		},
		"set_angular_velocity", [](PhysicsComponent& pc, float angularVelocity) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D set_angular_velocity with lua [{0}]", body.index1);
				return;
			}

			b2Body_SetAngularVelocity(body, angularVelocity);
		},
		"get_angular_velocity", [](PhysicsComponent& pc) -> float {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D get_angular_velocity with lua [{0}]", body.index1);
				return 0;
			}

			const auto& angularVelocity = b2Body_GetAngularVelocity(body);
			return angularVelocity;
		},
		"set_gravity_scale", [](PhysicsComponent& pc, float gravityScale) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D set_gravity_scale with lua [{0}]", body.index1);
				return;
			}

			b2Body_SetGravityScale(body, gravityScale);
		}
	);
}
