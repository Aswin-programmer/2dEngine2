#include "pch.h"

#include "ECS/COMPONENTS/PhysicsComponent.h"

#include "ECS/Registry.h"
#include "RENDERER/CORE/CoreEngineData.h"

// PhysicsComponent::PhysicsComponent(PhysicsComponent&& other) noexcept
//	: worldId(other.worldId),
//	bodyId(other.bodyId),
//	shapeId(other.shapeId),
//	m_pUserData(std::move(other.m_pUserData)),
//	m_InitialAttribs(std::move(other.m_InitialAttribs)) {
//	other.bodyId = b2_nullBodyId; // Invalidate source
//	other.shapeId = b2_nullShapeId;
// }
//
// PhysicsComponent& PhysicsComponent::operator=(PhysicsComponent&& other) noexcept {
//	if (this != &other) {
//		// Destroy current body if valid
//		if (b2Body_IsValid(bodyId)) {
//			b2DestroyBody(bodyId);
//		}
//		worldId = other.worldId;
//		bodyId = other.bodyId;
//		shapeId = other.shapeId;
//		m_pUserData = std::move(other.m_pUserData);
//		m_InitialAttribs = std::move(other.m_InitialAttribs);
//		// Invalidate source
//		other.bodyId = b2_nullBodyId;
//		other.shapeId = b2_nullShapeId;
//	}
//	return *this;
// }

PhysicsComponent::PhysicsComponent( const b2WorldId worldID, const PhysicsAttributes& physicsAttr )
	: worldId( worldID ),
	  m_InitialAttribs( physicsAttr ),
	  m_pUserData{ nullptr },
	  m_pBody{ nullptr },
	  shapeId( b2_nullShapeId )
{
}

// PhysicsComponent::~PhysicsComponent()
//{
//	/*if (b2Body_IsValid(bodyId))
//	{
//		b2DestroyBody(bodyId);
//	}*/
//
// }

void PhysicsComponent::Init( int windowWidth, int windowHeight )
{
	if ( !b2World_IsValid( worldId ) )
	{
		LOG_ERROR( "Failed to create the Box2D world [{0}]", worldId.index1 );
	}

	bool bCircle{ m_InitialAttribs.bCircle };

	// Create the body def
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = static_cast<b2BodyType>( m_InitialAttribs.eType );
	auto PIXELS_TO_METERS = CoreEngineData::GetInstance().PixelsToMeters();
	bodyDef.position = {
		( m_InitialAttribs.position.x + m_InitialAttribs.offset.x - ( windowWidth * 0.5f ) +
			( ( bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.x ) *
				m_InitialAttribs.scale.x ) *
				0.5f ) *
			PIXELS_TO_METERS,
		( m_InitialAttribs.position.y + m_InitialAttribs.offset.y - ( windowHeight * 0.5f ) +
			( ( bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.y ) *
				m_InitialAttribs.scale.y ) *
				0.5f ) *
			PIXELS_TO_METERS };
	bodyDef.gravityScale = m_InitialAttribs.gravityScale;
	bodyDef.fixedRotation = m_InitialAttribs.bFixedRotation;

	// Create the Ridgid Body:
	m_pBody = MakeSharedBody( worldId, &bodyDef );
	if ( !m_pBody || !b2Body_IsValid( m_pBody->bodyId ) )
	{
		LOG_ERROR( "Failed to create the Box2D body [{0}]", m_pBody ? m_pBody->bodyId.index1 : -1 );
		return;
	}

	// Create the shape:
	b2Circle circleShape{};
	b2Polygon polyShape{};

	if ( m_InitialAttribs.bCircle )
	{
		circleShape.radius = PIXELS_TO_METERS * m_InitialAttribs.radius * m_InitialAttribs.scale.x;
		circleShape.center = { 0.0f, 0.0f };
	}
	else if ( m_InitialAttribs.bBoxShape )
	{
		polyShape = b2MakeBox(
			PIXELS_TO_METERS * m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f,
			PIXELS_TO_METERS * m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f );
	}
	else
	{
		// TODO: Create your polygon shape
	}

	// Create the user data
	m_pUserData = std::make_shared<UserData>();
	m_pUserData->userData = m_InitialAttribs.objectData;
	m_pUserData->type_id = entt::type_hash<ObjectData>::value();

	// Create a surface material.
	b2SurfaceMaterial material = {};

	// Create the fixture def
	b2ShapeDef fixtureDef = b2DefaultShapeDef();
	fixtureDef.density = m_InitialAttribs.density;
	fixtureDef.material.friction = m_InitialAttribs.friction;
	fixtureDef.material.restitution = m_InitialAttribs.restitution;
	// fixtureDef.isSensor = m_InitialAttribs.bIsSensor;
	// fixtureDef.enableContactEvents = m_InitialAttribs.bIsContactEventsEnabled;
	fixtureDef.userData = reinterpret_cast<void*>( m_pUserData.get() ); // TODO:: Some problem could be present.

	if ( m_InitialAttribs.bInteractionType == BodyInteractionType::COLLIDER )
	{
		fixtureDef.enableContactEvents = true;
	}
	else if ( m_InitialAttribs.bInteractionType == BodyInteractionType::SENSOR )
	{
		fixtureDef.isSensor = true;
		fixtureDef.enableSensorEvents = true;
	}
	else if ( m_InitialAttribs.bInteractionType == BodyInteractionType::TRIGGER )
	{
		fixtureDef.enableContactEvents = true;
		b2ShapeDef tempSensorBodyDef = b2DefaultShapeDef();
		tempSensorBodyDef.userData = reinterpret_cast<void*>( m_pUserData.get() );
		tempSensorBodyDef.enableSensorEvents = true;
		if ( m_InitialAttribs.bCircle )
		{
			b2CreateCircleShape( m_pBody->bodyId, &tempSensorBodyDef, &circleShape );
		}
		else
		{
			b2CreatePolygonShape( m_pBody->bodyId, &tempSensorBodyDef, &polyShape );
		}
	}

	if ( m_InitialAttribs.bCircle )
	{
		shapeId = b2CreateCircleShape( m_pBody->bodyId, &fixtureDef, &circleShape );
	}
	else
	{
		shapeId = b2CreatePolygonShape( m_pBody->bodyId, &fixtureDef, &polyShape );
	}

	if ( !b2Shape_IsValid( shapeId ) )
	{
		LOG_ERROR( "Failed to create the Box2D shape [{0}]", shapeId.index1 );
	}
}

glm::vec2 PhysicsComponent::BodyPosition()
{
	if ( !b2Body_IsValid( m_pBody->bodyId ) )
		return { 0, 0 };
	const b2Vec2 pos = b2Body_GetPosition( m_pBody->bodyId );
	auto METERS_TO_PIXELS = CoreEngineData::GetInstance().MetersToPixels();
	return { pos.x * METERS_TO_PIXELS, pos.y * METERS_TO_PIXELS };
}

const bool PhysicsComponent::IsSensor() const
{
	return b2Shape_IsSensor( shapeId );
}

void PhysicsComponent::CreatePhysicsLuaBind( sol::state& lua, Registry& registry )
{
	lua.new_usertype<ObjectData>(
		"ObjectData",
		"type_id", entt::type_hash<ObjectData>::value,
		sol::call_constructor,
		sol::factories(
			[]( const std::string& tag, const std::string& group,
				bool bCollider, bool bTrigger, bool bSensor, std::uint32_t entityID ) {
				return ObjectData{
					.tag = tag,
					.group = group,
					.bCollider = bCollider,
					.bTrigger = bTrigger,
					.bSensor = bSensor,
					.entityID = entityID };
			},
			[]( const sol::table& objectData ) {
				return ObjectData{
					.tag = objectData["tag"].get_or( std::string{ "" } ),
					.group = objectData["group"].get_or( std::string{ "" } ),
					.bCollider = objectData["bCollider"].get_or( false ),
					.bTrigger = objectData["bTrigger"].get_or( false ),
					.bSensor = objectData["bSensor"].get_or( false ),
					.entityID = objectData["entityID"].get_or( (std::uint32_t)entt::null ) };
			} ),
		"tag", &ObjectData::tag,
		"group", &ObjectData::group,
		"bCollider", &ObjectData::bCollider,
		"bTrigger", &ObjectData::bTrigger,
		"bSensor", &ObjectData::bSensor,
		"entityID", &ObjectData::entityID,
		"contactEntities", &ObjectData::contactEntities,
		"to_string", &ObjectData::to_string );

	lua.new_enum<RigidBodyType>(
		"BodyType", { { "Static", RigidBodyType::STATIC },
						{ "Kinematic", RigidBodyType::KINEMATIC },
						{ "Dynamic", RigidBodyType::DYNAMIC } } );

	lua.new_enum<BodyInteractionType>(
		"BodyInteractionType", { { "NORMAL", BodyInteractionType::NORMAL },
								   { "COLLIDER", BodyInteractionType::COLLIDER },
								   { "SENSOR", BodyInteractionType::SENSOR },
								   { "TRIGGER", BodyInteractionType::TRIGGER } } );

	lua.new_usertype<PhysicsAttributes>(
		"PhysicsAttributes",
		sol::call_constructor,
		sol::factories(
			[] {
				return PhysicsAttributes{};
			},
			[]( const sol::table& physAttr ) {
				return PhysicsAttributes{
					.eType = physAttr["eType"].get_or( RigidBodyType::STATIC ),
					.bInteractionType = physAttr["bInteractionType"].get_or( BodyInteractionType::NORMAL ),
					.density = physAttr["density"].get_or( 100.f ),
					.friction = physAttr["friction"].get_or( 0.2f ),
					.restitution = physAttr["restitution"].get_or( 0.2f ),
					.restitutionThreshold = physAttr["restitutionThreshold"].get_or( 0.2f ),
					.radius = physAttr["radius"].get_or( 0.f ),
					.gravityScale = physAttr["gravityScale"].get_or( 1.f ),
					.position = glm::vec2{
						physAttr["position"]["x"].get_or( 0.f ),
						physAttr["position"]["y"].get_or( 0.f ) },
					.scale = glm::vec2{ physAttr["scale"]["x"].get_or( 0.f ), physAttr["scale"]["y"].get_or( 0.f ) },
					.boxSize = glm::vec2{ physAttr["boxSize"]["x"].get_or( 0.f ), physAttr["boxSize"]["y"].get_or( 0.f ) },
					.offset = glm::vec2{ physAttr["offset"]["x"].get_or( 0.f ), physAttr["offset"]["y"].get_or( 0.f ) },
					.bCircle = physAttr["bCircle"].get_or( false ),
					.bBoxShape = physAttr["bBoxShape"].get_or( true ),
					.bFixedRotation = physAttr["bFixedRotation"].get_or( true ),
					.bIsSensor = physAttr["bIsSensor"].get_or( false ),
					.bIsContactEventsEnabled = physAttr["bIsContactEventsEnabled"].get_or( false ),
					.filterCategory = physAttr["filterCategory"].get_or( (uint16_t)0 ),
					.filterMask = physAttr["filterMask"].get_or( (uint16_t)0 ),
					.objectData = ObjectData{ .tag = physAttr["objectData"]["tag"].get_or( std::string{ "" } ), .group = physAttr["objectData"]["group"].get_or( std::string{ "" } ), .bCollider = physAttr["objectData"]["bCollider"].get_or( false ), .bTrigger = physAttr["objectData"]["bTrigger"].get_or( false ), .bSensor = physAttr["objectData"]["bSensor"].get_or( false ), .entityID = physAttr["objectData"]["entityID"].get_or( (std::uint32_t)0 ) } };
			}
			// TODO: Add more specific ctor
			),
		"eType", &PhysicsAttributes::eType,
		"bInteractionType", &PhysicsAttributes::bInteractionType,
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
		"bFixedRotation", &PhysicsAttributes::bFixedRotation,
		"bIsSensor", &PhysicsAttributes::bIsSensor,
		"bIsContactEventsEnabled", &PhysicsAttributes::bIsContactEventsEnabled,
		"objectData", &PhysicsAttributes::objectData
		// TODO: Add in filters and other properties as needed
	);

	auto& box2DWrapper = registry.GetContext<std::shared_ptr<Box2DWrappers>>();
	if ( !box2DWrapper )
	{
		LOG_ERROR( "Failed to get box2DWrapper in PhysicsComponent.cpp" );
	}

	lua.new_usertype<PhysicsComponent>(
		"PhysicsComp",
		"type_id", &entt::type_hash<PhysicsComponent>::value,
		sol::call_constructor,
		sol::factories(
			[&]( const PhysicsAttributes& attr ) {
				PhysicsComponent pc{ box2DWrapper->GetWorldID(), attr };
				pc.Init( 640, 480 ); // TODO: Change based on window values
				return pc;
			} ),
		"linear_impulse", []( PhysicsComponent& pc, const glm::vec2& impulse ) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				LOG_ERROR("Failed to bind the Box2D linear_impulse with lua [{0}]", body.index1);
				return;
			}
			// Get the position in meters directly from Box2D
			b2Vec2 center = b2Body_GetPosition(body);
			b2Body_ApplyLinearImpulse(body, b2Vec2{ impulse.x, impulse.y }, center, true); },
		"angular_impulse", []( PhysicsComponent& pc, float impulse ) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D angular_impulse with lua [{0}]", body.index1);
				return;
			}

			b2Body_ApplyAngularImpulse(body, impulse, true); },
		"set_linear_velocity", []( PhysicsComponent& pc, const glm::vec2& velocity ) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D set_linear_velocity with lua [{0}]", body.index1);
				return;
			}

			b2Body_SetLinearVelocity(body, b2Vec2{ velocity.x, velocity.y }); },
		"get_linear_velocity", []( PhysicsComponent& pc ) -> glm::vec2 {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D get_linear_velocity with lua [{0}]", body.index1);
				return glm::vec2(0.0f);
			}

			const auto& linearVelocity = b2Body_GetLinearVelocity(body);
			return glm::vec2{ linearVelocity.x, linearVelocity.y }; },
		"set_angular_velocity", []( PhysicsComponent& pc, float angularVelocity ) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D set_angular_velocity with lua [{0}]", body.index1);
				return;
			}

			b2Body_SetAngularVelocity(body, angularVelocity); },
		"get_angular_velocity", []( PhysicsComponent& pc ) -> float {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D get_angular_velocity with lua [{0}]", body.index1);
				return 0;
			}

			const auto& angularVelocity = b2Body_GetAngularVelocity(body);
			return angularVelocity; },
		"set_gravity_scale", []( PhysicsComponent& pc, float gravityScale ) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D set_gravity_scale with lua [{0}]", body.index1);
				return;
			}

			b2Body_SetGravityScale(body, gravityScale); },
		"apply_force_center", []( PhysicsComponent& pc, const glm::vec2& force ) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Failed to bind the Box2D set_gravity_scale with lua [{0}]", body.index1);
				return;
			}

			b2Body_ApplyForceToCenter(body, { force.x,force.y }, true); },
		"set_transform", []( PhysicsComponent& pc, const glm::vec2& position ) {
			auto body = pc.getBodyID();
			if (!b2Body_IsValid(body))
			{
				// TODO: Add Error
				LOG_ERROR("Invalid bodyID passed in set_transform function in PhysicsComponent");
				return;
			}

			auto& engineData = CoreEngineData::GetInstance();
			const auto p2m = engineData.PixelsToMeters();

			const auto scaleHalfHeight = engineData.ScaledHeight() * 0.5f;
			const auto scaleHalfWidth = engineData.ScaledWidth() * 0.5f;

			auto bx = (position.x * p2m) - scaleHalfWidth;
			auto by = (position.y * p2m) - scaleHalfHeight;
			b2Body_SetTransform(body, b2Vec2{ bx, by }, b2Rot{.c = 1.f, .s = 0.f } ); } );
}
