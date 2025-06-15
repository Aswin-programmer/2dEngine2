#include "pch.h"

#include "SYSTEMS/PhysicsSystem.h"

#include "ECS/Registry.h"
#include "ECS/Components/BoxColliderComponent.h"
#include "ECS/Components/CircleColliderComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/PhysicsComponent.h"
// #include "LOGGER/log.h"
#include "RENDERER/CORE/CoreEngineData.h"

PhysicsSystem::PhysicsSystem( Registry& registry )
	: m_Registry{ registry }
{
}

void PhysicsSystem::Update( entt::registry& registry )
{
	// Setup for PhysicsComponent with BoxColliderComponent
	auto boxView = registry.view<PhysicsComponent, TransformComponent, BoxColliderComponent>();
	auto& coreEngine = CoreEngineData::GetInstance();

	float hScaledWidth = coreEngine.ScaledWidth() * 0.5f;
	float hScaledHeight = coreEngine.ScaledHeight() * 0.5f;

	for ( auto entity : boxView )
	{
		auto& physics = boxView.get<PhysicsComponent>( entity );
		auto pRigitBoxBodyID = physics.getBodyID();

		if ( !b2Body_IsValid( pRigitBoxBodyID ) )
		{
			LOG_WARN( "Failed to create the Box2D body [{0}]", pRigitBoxBodyID.index1 );
			continue;
		}

		auto& transform = boxView.get<TransformComponent>( entity );
		auto& boxCollider = boxView.get<BoxColliderComponent>( entity );

		const auto& bodyPosition = b2Body_GetPosition( pRigitBoxBodyID );
		transform.position.x = ( hScaledWidth + bodyPosition.x ) * coreEngine.MetersToPixels() -
							   ( boxCollider.width * transform.scale.x ) * 0.5f - boxCollider.offset.x;

		transform.position.y = ( hScaledHeight + bodyPosition.y ) * coreEngine.MetersToPixels() -
							   ( boxCollider.height * transform.scale.y ) * 0.5f - boxCollider.offset.y;
		if ( !b2Body_IsFixedRotation( pRigitBoxBodyID ) )
		{
			transform.rotation = glm::degrees(
				b2Rot_GetAngle( b2Body_GetRotation( pRigitBoxBodyID ) ) );
		}
	}

	// Setup for PhysicsComponent with CircleColliderComponent
	auto circleView = registry.view<PhysicsComponent, TransformComponent, CircleColliderComponent>();
	for ( auto entity : circleView )
	{
		auto& physics = circleView.get<PhysicsComponent>( entity );
		auto pRigitCircleBodyID = physics.getBodyID();

		if ( !b2Body_IsValid( pRigitCircleBodyID ) )
		{
			LOG_WARN( "Failed to create the Box2D body [{0}]", pRigitCircleBodyID.index1 );
			continue;
		}

		auto& transform = circleView.get<TransformComponent>( entity );
		auto& circleCollider = circleView.get<CircleColliderComponent>( entity );

		const auto& bodyPosition = b2Body_GetPosition( pRigitCircleBodyID );
		transform.position.x = ( hScaledWidth + bodyPosition.x ) * coreEngine.MetersToPixels() -
							   ( circleCollider.radius * transform.scale.x ) - circleCollider.offset.x;

		transform.position.y = ( hScaledHeight + bodyPosition.y ) * coreEngine.MetersToPixels() -
							   ( circleCollider.radius * transform.scale.y ) - circleCollider.offset.y;

		if ( !b2Body_IsFixedRotation( pRigitCircleBodyID ) )
		{
			transform.rotation = glm::degrees(
				b2Rot_GetAngle( b2Body_GetRotation( pRigitCircleBodyID ) ) );
		}
	}
}
