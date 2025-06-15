#include "TOOLS/RectFillTool.h"
#include "UTILITIES/EditorUtilities.h"

#include "ECS/Entity.h"
#include "ECS/MainRegistry.h"
#include "RENDERER/CORE/Camera2D.h"
#include "RENDERER/CORE/BatchRenderer.h"
#include "RENDERER/CORE/RectBatchRenderer.h"
#include "RENDERER/ESSENTIALS/Primitives.h"
#include "RENDERER/SHADER/Shader.h";
#include "RENDERER/TEXTURE/Texture.h";
#include "RESOURCES/AssetManager.h"

void RectFillTool::CreateTiles()
{
	const auto& sprite = m_pMouseTile->sprite;
	const auto& transform = m_pMouseTile->transform;
	auto& pTexture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
	assert( pTexture.getID() != 0 && "Texture doesn't exist!" );

	const int dx = static_cast<int>( m_pTileFillRect->width );
	const int dy = static_cast<int>( m_pTileFillRect->height );

	auto spriteWidth = static_cast<int>( sprite.width * transform.scale.x * ( dx > 0 ? 1.f : -1.f ) );
	auto spriteHeight = static_cast<int>( sprite.height * transform.scale.y * ( dy > 0 ? 1.f : -1.f ) );

	for ( int y = 0; ( dy > 0 ? y < dy : y > dy ); y += spriteHeight )
	{
		for ( int x = 0; ( dx > 0 ? x < dx : x > dx ); x += spriteWidth )
		{
			glm::vec2 newTIlePosition{ m_StartPressPos.x + x, m_StartPressPos.y + y };

			// Check if there is already a tile
			if ( auto id = CheckForTile( newTIlePosition ); id != entt::null )
			{
				continue;
			}

			Entity tile{ CreateEntity() };

			auto& newTransform = tile.AddComponent<TransformComponent>( transform );
			newTransform.position = newTIlePosition;
			tile.AddComponent<SpriteComponent>( sprite );

			if ( m_pMouseTile->bCollider )
			{
				// TODO : Add collider component
			}

			if ( m_pMouseTile->bAnimation )
			{
				// TODO : Add animation component
			}

			if ( m_pMouseTile->bPhysics )
			{
				// TODO : Add physics component
			}

			tile.AddComponent<TileComponent>( static_cast<uint32_t>( tile.GetEntity() ) );
		}
	}
}

void RectFillTool::RemoveTiles()
{
	const auto& sprite = m_pMouseTile->sprite;
	const auto& transform = m_pMouseTile->transform;

	const int dx = static_cast<int>( m_pTileFillRect->width );
	const int dy = static_cast<int>( m_pTileFillRect->height );

	auto spriteWidth = static_cast<int>( sprite.width * transform.scale.x * ( dx > 0 ? 1.f : -1.f ) );
	auto spriteHeight = static_cast<int>( sprite.height * transform.scale.y * ( dy > 0 ? 1.f : -1.f ) );

	std::set<std::uint32_t> entitiesToRemove{};

	for ( int y = 0; ( dy > 0 ? y < dy : y > dy ); y += spriteHeight )
	{
		for ( int x = 0; ( dx > 0 ? x < dx : x > dx ); x += spriteWidth )
		{
			// Check if there is already a tile
			if ( auto id = CheckForTile( glm::vec2{ m_StartPressPos.x + x, m_StartPressPos.y + y } ); id != entt::null )
			{
				entitiesToRemove.insert( id );
			}
		}
	}

	for ( auto id : entitiesToRemove )
	{
		Entity removedTile{ CreateEntity( id ) };
		removedTile.Kill();
	}
}

void RectFillTool::DrawPreview( int dx, int dy )
{
	const auto& sprite = m_pMouseTile->sprite;
	const auto& transform = m_pMouseTile->transform;
	auto& pTexture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
	assert( pTexture.getID() != 0 && "Texture doesn't exist!" );

	auto spriteWidth = static_cast<int>( sprite.width * transform.scale.x * ( dx > 0 ? 1.f : -1.f ) );
	auto spriteHeight = static_cast<int>( sprite.height * transform.scale.y * ( dy > 0 ? 1.f : -1.f ) );

	for ( int y = 0; ( dy > 0 ? y < dy : y > dy ); y += spriteHeight )
	{
		for ( int x = 0; ( dx > 0 ? x < dx : x > dx ); x += spriteWidth )
		{
			glm::vec4 tilePosition{
				m_StartPressPos.x + x,
				m_StartPressPos.y + y,
				sprite.width * transform.scale.x,
				sprite.height * transform.scale.y };

			glm::vec4 uvs{ sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height };

			m_pBatchRenderer->AddSprite( tilePosition, uvs, pTexture.getID(), PRVIEW_LAYER, glm::mat4{ 1.0f }, sprite.color );
		}
	}
}

void RectFillTool::ResetTile()
{
	*m_pTileFillRect = Rect{};
}

RectFillTool::RectFillTool()
	: TileTool(),
	  m_pShapeRenderer{ std::make_unique<RectBatchRenderer>() },
	  m_pTileFillRect{ std::make_unique<Rect>() },
	  m_StartPressPos{ 0 }
{
}

RectFillTool::~RectFillTool()
{
}

void RectFillTool::Create()
{
	if ( !CanDrawOrCreate() )
	{
		return;
	}

	if ( MouseBtnJustPressed( EMouseButton::LEFT ) || MouseBtnJustPressed( EMouseButton::RIGHT ) )
	{
		m_StartPressPos = GetMouseWorldCoords();
	}

	if ( MouseBtnJustReleased( EMouseButton::LEFT ) )
	{
		CreateTiles();
		ResetTile();
	}
	else if ( MouseBtnJustReleased( EMouseButton::RIGHT ) )
	{
		RemoveTiles();
		ResetTile();
	}
}

void RectFillTool::Draw()
{
	if ( !CanDrawOrCreate() )
	{
		return;
	}

	auto& pShader = MAIN_REGISTRY().GetAssetManager().GetShader( "shader1" );
	assert( pShader.getID() != 0 && "Basic shader doesn't exist!" );

	pShader.use();
	auto camMat = m_pCamera->GetCameraMatrix();
	pShader.setMat4( "uProjection", camMat );

	DrawMouseSprite();

	bool bLeftMousePressed{ MouseBtnPressed( EMouseButton::LEFT ) };
	bool bRightMousePressed{ MouseBtnPressed( EMouseButton::RIGHT ) };
	if ( !bLeftMousePressed && !bRightMousePressed )
	{
		return;
	}

	const auto& mouseWorldCords = GetMouseWorldCoords();
	auto dx = mouseWorldCords.x - m_StartPressPos.x;
	auto dy = mouseWorldCords.y - m_StartPressPos.y;

	const auto& sprite = m_pMouseTile->sprite;
	const auto& transform = m_pMouseTile->transform;

	auto spriteWidth = sprite.width * transform.scale.x;
	auto spriteHeight = sprite.height * transform.scale.y;

	dx += dx > 0 ? spriteWidth : -spriteWidth;
	dy += dy > 0 ? spriteHeight : -spriteHeight;

	auto newPosX = m_StartPressPos.x + ( dx > 0 ? 0 : spriteWidth );
	auto newPosY = m_StartPressPos.y + ( dy > 0 ? 0 : spriteHeight );

	auto& pColorShader = MAIN_REGISTRY().GetAssetManager().GetShader( "color" );
	assert( pColorShader.getID() != 0 && "Basic shader doesn't exist!" );

	Color color{}; // The default value of the color is being set as White. Check the Vertex.h file.

	if ( bLeftMousePressed )
	{
		color = { 0x7D, 0xF9, 0xFF, 100 };
		m_pBatchRenderer->Begin();
		DrawPreview( dx, dy );
		m_pBatchRenderer->End();
		m_pBatchRenderer->Render();
		pShader.unuse();
	}
	else if ( bRightMousePressed )
	{
		color = { 0xFF, 0x73, 0x77, 100 };
	}

	pColorShader.use();
	m_pShapeRenderer->Begin();
	m_pShapeRenderer->AddRect(
		Rect{
			.position = { newPosX, newPosY },
			.width = dx,
			.height = dy,
			.color = color } );
	m_pShapeRenderer->End();
	m_pShapeRenderer->Render();
	pColorShader.unuse();

	m_pTileFillRect->position = { newPosX, newPosY };
	m_pTileFillRect->width = dx;
	m_pTileFillRect->height = dy;
}
