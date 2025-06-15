#include "SYSTEMS/GridSystem.h"

#include "SCENE/SceneObject.h"

#include "ECS/MainRegistry.h"
#include "RENDERER/CORE/Camera2D.h"
#include "RENDERER/CORE/RectBatchRenderer.h"
#include "RENDERER/ESSENTIALS/Primitives.h"
#include "RENDERER/SHADER/Shader.h";
#include "RESOURCES/AssetManager.h"

GridSystem::GridSystem()
	: m_pBatchRenderer{ std::make_unique<RectBatchRenderer>() }
{
}

void GridSystem::Update( SceneObject& currentScene, Camera2D& camera )
{
	auto& assetManager = MAIN_REGISTRY().GetAssetManager();

	const auto& canvas = currentScene.GetCanvas();
	auto camMat = camera.GetCameraMatrix();

	auto& pColorShader = assetManager.GetShader( "color" );

	pColorShader.use();
	pColorShader.setMat4( "uProjection", camMat );

	m_pBatchRenderer->Begin();

	int tileWidth{ canvas.tileWidth }, tileHeight{ canvas.tileHeight }, canvasWidth{ canvas.width }, canvasHeight{ canvas.height };

	int cols = canvasWidth / tileWidth;
	int rows = canvasHeight / tileHeight;

	Color color{};

	for ( int row = 0; row < rows; row++ )
	{
		for ( int col = 0; col < cols; col++ )
		{
			// Create the checkboard colors
			if ( ( col - row ) % 2 == 0 && ( row - col ) % 2 == 0 )
				color = { 125, 125, 125, 70 };
			else
				color = { 200, 200, 200, 70 };

			Rect rect{
				.position = glm::vec2{
					static_cast<float>( col * tileWidth ),
					static_cast<float>( row * tileHeight ) },
				.width = static_cast<float>( tileWidth ),
				.height = static_cast<float>( tileHeight ),
				.color = color };

			m_pBatchRenderer->AddRect( rect );
		}
	}

	m_pBatchRenderer->End();
	m_pBatchRenderer->Render();

	pColorShader.unuse();
}
