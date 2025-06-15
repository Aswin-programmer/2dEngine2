#include "pch.h"

#include "SYSTEMS/RenderShapeSystem.h"

#include "ECS/MainRegistry.h"
#include "ECS/Components/BoxColliderComponent.h"
#include "ECS/Components/CircleColliderComponent.h"
#include "ECS/Components/PhysicsComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "RENDERER/SHADER/Shader.h";
#include "RENDERER/CORE/Camera2D.h"
#include "RENDERER/CORE/CircleBatchRenderer.h"
#include "RENDERER/CORE/CoreEngineData.h"
#include "RENDERER/CORE/RectBatchRenderer.h"
#include "RENDERER/ESSENTIALS/Primitives.h"
#include "RESOURCES/AssetManager.h"

RenderShapeSystem::RenderShapeSystem()
	: m_pRectRenderer{ std::make_unique<RectBatchRenderer>() }, m_pCircleRenderer{ std::make_unique<CircleBatchRenderer>() }
{
}

void RenderShapeSystem::Update( Registry& registry, Camera2D& camera )
{
	if ( !CoreEngineData::GetInstance().RenderCollidersEnabled() )
		return;

	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();

	auto& colorShader = assetManager.GetShader( "color" );
	auto cam_mat = camera.GetCameraMatrix();

	colorShader.use();
	colorShader.setMat4( "uProjection", cam_mat );
	m_pRectRenderer->Begin();

	auto boxView = registry.GetRegistry().view<TransformComponent, BoxColliderComponent>();
	for ( auto entity : boxView )
	{
		const auto& transform = boxView.get<TransformComponent>( entity );
		const auto& boxCollider = boxView.get<BoxColliderComponent>( entity );

		glm::mat4 model{ 1.f };

		if ( transform.rotation > 0.f || transform.rotation < 0.f ||
			 transform.scale.x > 1.f || transform.scale.x < 1.f ||
			 transform.scale.y > 1.f || transform.scale.y < 1.f )
		{
			model = glm::translate( model, glm::vec3{ transform.position, 0.f } );
			model = glm::translate( model, glm::vec3{ ( boxCollider.width * transform.scale.x ) * 0.5f, ( boxCollider.height * transform.scale.y ) * 0.5f, 0.f } );

			model = glm::rotate( model, glm::radians( transform.rotation ), glm::vec3{ 0.f, 0.f, 1.f } );
			model = glm::translate( model, glm::vec3{ ( boxCollider.width * transform.scale.x ) * -0.5f, ( boxCollider.height * transform.scale.y ) * -0.5f, 0.f } );

			model = glm::scale( model, glm::vec3{ transform.scale, 1.f } );

			model = glm::translate( model, glm::vec3{ -transform.position, 0.f } );
		}

		auto color = Color{ 255, 0, 0, 135 };

		if ( registry.GetRegistry().all_of<PhysicsComponent>( entity ) )
		{
			auto& physics = registry.GetRegistry().get<PhysicsComponent>( entity );
			if ( physics.IsSensor() )
				color = Color{ 0, 255, 0, 135 };
		}

		Rect rect{
			.position = glm::vec2{
				transform.position.x + boxCollider.offset.x,
				transform.position.y + boxCollider.offset.y },
			.width = static_cast<float>( boxCollider.width ),
			.height = static_cast<float>( boxCollider.height ),
			.color = color };

		m_pRectRenderer->AddRect( rect, model );
	}

	m_pRectRenderer->End();
	m_pRectRenderer->Render();
	colorShader.unuse();

	// Don't forget to use the & symbol here. Its creates a lot error!. The object will be not be proper.
	auto& circleShader = assetManager.GetShader( "circle" );

	circleShader.use();
	circleShader.setMat4( "uProjection", cam_mat );
	m_pCircleRenderer->Begin();

	auto circleView = registry.GetRegistry().view<TransformComponent, CircleColliderComponent>();
	for ( auto entity : circleView )
	{
		const auto& transform = circleView.get<TransformComponent>( entity );
		const auto& circleCollider = circleView.get<CircleColliderComponent>( entity );

		glm::vec4 circle{
			transform.position.x + circleCollider.offset.x,
			transform.position.y + circleCollider.offset.y,
			circleCollider.radius * transform.scale.x * 2,
			circleCollider.radius * transform.scale.y * 2 };

		m_pCircleRenderer->AddCircle( circle, Color{ 0, 255, 0, 135 }, 1.f );
	}

	m_pCircleRenderer->End();
	m_pCircleRenderer->Render();
	circleShader.unuse();
}
