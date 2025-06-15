#include "pch.h"

#include "SYSTEMS/RenderSystem.h"

#include "ECS/MainRegistry.h"
#include "ECS/Registry.h"
#include "ECS/COMPONENTS/SpriteComponent.h"
#include "ECS/COMPONENTS/TransformComponent.h"
#include "LOGGER/log.h"
#include "RENDERER/CORE/BatchRenderer.h"
#include "RENDERER/CORE/Camera2D.h"
#include "RENDERER/SHADER/Shader.h"
#include "RENDERER/TEXTURE/Texture.h"
#include "RESOURCES/AssetManager.h"

RenderSystem::RenderSystem()
	: m_pBatchRenderer{ nullptr }
{
	m_pBatchRenderer = std::make_unique<SpriteBatchRenderer>();
}

void RenderSystem::Update( Registry& registry, Camera2D& camera )
{
	auto view = registry.GetRegistry().view<SpriteComponent, TransformComponent>();
	if ( view.size_hint() < 1 )
	{
		return;
	}

	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();
	auto& spriteShader = assetManager.GetShader( "shader1" );
	auto cam_mat = camera.GetCameraMatrix();
	if ( spriteShader.getID() == 0 )
	{
		LOG_ERROR( "Sprite shader program has not been set correctly!" );
		return;
	}
	// enable the shader
	spriteShader.use();
	spriteShader.setMat4( "uProjection", cam_mat );
	m_pBatchRenderer->Begin();
	// auto view = registry.GetRegistry().view<SpriteComponent, TransformComponent>();
	for ( const auto& entity : view )
	{
		const auto& transform = view.get<TransformComponent>( entity );
		const auto& sprite = view.get<SpriteComponent>( entity );

		////Checking Whether Entity Is Present Within the Camera View Or Not:
		// const auto& cameraPos = camera.GetPosition() - camera.GetScreenOffset();
		// const auto& cameraWidth = camera.GetWidth();
		// const auto& cameraHeight = camera.GetHeight();
		// const auto& cameraScale = camera.GetScale();

		// if (
		//	(transform.position.x <= ((cameraPos.x - (sprite.width * transform.scale.x * cameraScale)) / cameraScale))
		//	||
		//	(transform.position.x >= ((cameraPos.x + cameraWidth) / cameraScale))
		//	||
		//	(transform.position.y <= ((cameraPos.y - (sprite.height * transform.scale.y * cameraScale)) / cameraScale))
		//	||
		//	(transform.position.y >= ((cameraPos.y + cameraHeight) / cameraScale))
		//	)
		//{
		//	return;
		// }

		if ( sprite.texture_name.empty() || sprite.bHidden )
			continue;
		const auto& texture = assetManager.GetTexture( sprite.texture_name );
		if ( texture.getID() == 0 )
		{
			LOG_ERROR( "Texture [{0}] was not created correctly!", sprite.texture_name );
			return;
		}

		glm::vec4 spriteRect{ transform.position.x, transform.position.y, sprite.width, sprite.height };
		glm::vec4 uvRect{ sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height };
		glm::mat4 model{ 1.f };
		if ( transform.rotation > 0.f || transform.rotation < 0.f ||
			 transform.scale.x > 1.f || transform.scale.x < 1.f ||
			 transform.scale.y > 1.f || transform.scale.y < 1.f )
		{
			model = glm::translate( model, glm::vec3{ transform.position, 0.f } );
			model = glm::translate( model, glm::vec3{ ( sprite.width * transform.scale.x ) * 0.5f, ( sprite.height * transform.scale.y ) * 0.5f, 0.f } );
			model = glm::rotate( model, glm::radians( transform.rotation ), glm::vec3{ 0.f, 0.f, 1.f } );
			model = glm::translate( model, glm::vec3{ ( sprite.width * transform.scale.x ) * -0.5f, ( sprite.height * transform.scale.y ) * -0.5f, 0.f } );
			model = glm::scale( model, glm::vec3{ transform.scale, 1.f } );
			model = glm::translate( model, glm::vec3{ -transform.position, 0.f } );
		}
		m_pBatchRenderer->AddSprite( spriteRect, uvRect, texture.getID(), sprite.layer, model, sprite.color );
	}
	m_pBatchRenderer->End();
	m_pBatchRenderer->Render();
	spriteShader.unuse();
}
