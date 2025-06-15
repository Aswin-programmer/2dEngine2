#include "pch.h"

#include "SYSTEMS/RenderUISystem.h"

#include "ECS/MainRegistry.h"
#include "ECS/COMPONENTS/TextComponent.h"
#include "ECS/COMPONENTS/TransformComponent.h"
#include "LOGGER/log.h"
#include "RENDERER/CORE/BatchRenderer.h"
#include "RENDERER/CORE/Camera2D.h"
#include "RENDERER/CORE/TextBatchRenderer.h"

#include "RENDERER/SHADER/Shader.h";

#include "RESOURCES/AssetManager.h"

RenderUISystem::RenderUISystem()
	: m_pSpriteRenderer{ std::make_unique<SpriteBatchRenderer>() }, m_pTextRenderer{ std::make_unique<TextBatchRenderer>() }, m_pCamera2D{ nullptr }
{
	m_pCamera2D = std::make_unique<Camera2D>( 640, 480 );
	m_pCamera2D->Update();
}

void RenderUISystem::Update( Registry& registry, Camera2D& camera )
{
	auto& mainRegistry = MAIN_REGISTRY();

	// If there are no entities in the view, leave
	auto textView = registry.GetRegistry().view<TextComponent, TransformComponent>();
	if ( textView.size_hint() < 1 )
		return;

	auto& assetManager = mainRegistry.GetAssetManager();

	auto& pFontShader = assetManager.GetShader( "font" );

	/*if (!pFontShader)
	{
		LOG_ERROR("Failed to get the font shader from the asset manager!");
		return;
	}*/

	auto cam_mat = camera.GetCameraMatrix();
	// auto textView = registry.GetRegistry().view<TextComponent, TransformComponent>();

	pFontShader.use();
	pFontShader.setMat4( "uProjection", cam_mat );

	m_pTextRenderer->Begin();

	for ( auto entity : textView )
	{
		const auto& text = textView.get<TextComponent>( entity );

		if ( text.sFontName.empty() || text.bHidden )
			continue;

		const auto& pFont = assetManager.GetFont( text.sFontName );
		if ( !pFont )
		{
			LOG_ERROR( "Font [{}] does not exist in the asset manager!", text.sFontName );
			continue;
		}

		const auto& transform = textView.get<TransformComponent>( entity );

		glm::mat4 model{ 1.f };

		if ( transform.rotation > 0.f || transform.rotation < 0.f ||
			 transform.scale.x > 1.f || transform.scale.x < 1.f ||
			 transform.scale.y > 1.f || transform.scale.y < 1.f )
		{
			model = glm::translate( model, glm::vec3{ transform.position, 0.f } );
			model = glm::translate( model, glm::vec3{ ( pFont->GetFontSize() * transform.scale.x ) * 0.5f, ( pFont->GetFontSize() * transform.scale.y ) * 0.5f, 0.f } );

			model = glm::rotate( model, glm::radians( transform.rotation ), glm::vec3{ 0.f, 0.f, 1.f } );
			model = glm::translate( model, glm::vec3{ ( pFont->GetFontSize() * transform.scale.x ) * -0.5f, ( pFont->GetFontSize() * transform.scale.y ) * -0.5f, 0.f } );

			model = glm::scale( model, glm::vec3{ transform.scale, 1.f } );

			model = glm::translate( model, glm::vec3{ -transform.position, 0.f } );
		}

		m_pTextRenderer->AddText( text.sTextStr, pFont, transform.position, text.padding, text.wrap, text.color, model );
	}

	m_pTextRenderer->End();
	m_pTextRenderer->Render();

	pFontShader.unuse();
}
