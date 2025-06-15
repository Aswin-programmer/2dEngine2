#include "TOOLS/CreateTileTool.h"

#include "UTILITIES/EditorUtilities.h"

#include "ECS/Entity.h"
#include "ECS/MainRegistry.h"
#include "RENDERER/CORE/Camera2D.h"
#include "RENDERER/SHADER/Shader.h";
#include "RESOURCES/AssetManager.h"

void CreateTileTool::CreateNewTile()
{
	const auto& mouseWorldCoords = GetMouseWorldCoords();

	// Check if there is already a tile
	if ( auto id = CheckForTile( mouseWorldCoords ); id != entt::null )
	{
		return;
	}

	Entity tile{ CreateEntity() };

	tile.AddComponent<TransformComponent>( m_pMouseTile->transform );
	tile.AddComponent<SpriteComponent>( m_pMouseTile->sprite );

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

void CreateTileTool::RemoveTile()
{
	const auto& mouseWordCoords = GetMouseWorldCoords();

	// Check if there is a tile that we can remove
	if ( auto id = CheckForTile( mouseWordCoords ); id != entt::null )
	{
		// Create an entity from that id
		Entity tileToRemove{ CreateEntity( id ) };

		// TODO: We need to eventually get all of the component information fromAdd commentMore actions
		// the tile entity so we can place it into the Undo/Redo Commands that will be
		// coming in the future. For now we will just delete the tile.
		tileToRemove.Kill();
	}
}

CreateTileTool::CreateTileTool()
	: TileTool()
{
}

CreateTileTool::~CreateTileTool()
{
}

void CreateTileTool::Create()
{
	if ( OutOfBounds() || !IsActivated() || !SpriteValid() || !m_pRegistry )
	{
		return;
	}

	if ( MouseBtnJustPressed( EMouseButton::LEFT ) || MouseBtnPressed( EMouseButton::LEFT ) )
	{
		CreateNewTile();
	}
	else if ( MouseBtnJustPressed( EMouseButton::RIGHT ) || MouseBtnPressed( EMouseButton::RIGHT ) )
	{
		RemoveTile();
	}
}

void CreateTileTool::Draw()
{
	if ( !SpriteValid() || !IsActivated() || OutOfBounds() || !m_pCamera )
	{
		return;
	}

	auto& pShader = MAIN_REGISTRY().GetAssetManager().GetShader( "shader1" );
	assert( pShader.getID() != 0 && "Basic shader doesn't exist!" );

	pShader.use();
	auto camMat = m_pCamera->GetCameraMatrix();
	pShader.setMat4( "uProjection", camMat );

	DrawMouseSprite();

	pShader.unuse();
}
