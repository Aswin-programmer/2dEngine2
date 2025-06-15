#include "TOOLS/TileTool.h"

#include "UTILITIES/EditorUtilities.h"

#include "ECS/Entity.h"
#include "ECS/MainRegistry.h"
#include "RENDERER/CORE/BatchRenderer.h"
#include "RENDERER/CORE/Camera2D.h"
#include "RENDERER/TEXTURE/Texture.h";
#include "RESOURCES/AssetManager.h"

uint32_t TileTool::CheckForTile( const glm::vec2& position )
{
	if ( !m_pRegistry )
	{
		return entt::null;
	}

	auto tileView = m_pRegistry->GetRegistry().view<TileComponent, TransformComponent>();
	for ( auto entity : tileView )
	{
		Entity tile{ *m_pRegistry, entity };
		const auto& transform = tile.GetComponent<TransformComponent>();
		const auto& sprite = tile.GetComponent<SpriteComponent>();

		if (
			( ( position.x >= transform.position.x ) && ( position.x < transform.position.x + sprite.width * transform.scale.x ) ) &&
			( ( position.y >= transform.position.y ) && ( position.y < transform.position.y + sprite.height * transform.scale.y ) ) &&
			( m_pMouseTile->sprite.layer == sprite.layer ) )
		{
			return static_cast<uint32_t>( entity );
		}
	}

	return entt::null;
}

Entity TileTool::CreateEntity()
{
	assert( m_pRegistry && "The registry must be valid to create an entity." );
	return Entity( *m_pRegistry, "", "" );
}

Entity TileTool::CreateEntity( uint32_t id )
{
	assert( m_pRegistry && "The registry must be valid to create an entity." );
	return Entity( *m_pRegistry, static_cast<entt::entity>( id ) );
}

void TileTool::DrawMouseSprite()
{
	m_pBatchRenderer->Begin();
	const auto& sprite = m_pMouseTile->sprite;
	const auto& transform = m_pMouseTile->transform;

	glm::vec4 position{
		transform.position.x,
		transform.position.y,
		sprite.width * transform.scale.x,
		sprite.height * transform.scale.y };

	glm::vec4 uvs{
		sprite.uvs.u,
		sprite.uvs.v,
		sprite.uvs.uv_width,
		sprite.uvs.uv_height };

	const auto& pTexture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
	m_pBatchRenderer->AddSprite( position, uvs, pTexture.getID(), MOUSE_SPRITE_LAYER, glm::mat4( 1.f ), sprite.color );

	m_pBatchRenderer->End();
	m_pBatchRenderer->Render();
}

void TileTool::ExamineMousePosition()
{
	if ( !m_pCamera )
	{
		return;
	}
	const auto& mouseWorldPos = GetMouseWorldCoords();
	auto& transform = m_pMouseTile->transform;

	const float cameraScale = m_pCamera->GetScale();
	bool bIsOffset{ true };

	if ( m_bGridSnap )
	{
		glm::vec2 mouseGrid{
			mouseWorldPos.x / ( m_MouseRect.x * transform.scale.x ),
			mouseWorldPos.y / ( m_MouseRect.y * transform.scale.y ) };

		auto scaledGridToCamX = std::floor( mouseGrid.x );
		auto scaledGridToCamY = std::floor( mouseGrid.y );

		transform.position.x = scaledGridToCamX * m_MouseRect.x * transform.scale.x;
		transform.position.y = scaledGridToCamY * m_MouseRect.y * transform.scale.y;

		m_GridCoords.x = scaledGridToCamX;
		m_GridCoords.y = scaledGridToCamY;

		SetMouseWorldCoords( transform.position );
	}
	else
	{
		transform.position = bIsOffset ? ( mouseWorldPos - m_MouseRect * 0.5f ) : mouseWorldPos;
	}
}

TileTool::TileTool()
	: AbstractTool(),
	  m_MouseRect{ 16.f },
	  m_GridCoords{ 0.f },
	  m_bGridSnap{ true },
	  m_pBatchRenderer{ std::make_shared<SpriteBatchRenderer>() },
	  m_pMouseTile{ std::make_shared<Tile>() }
{
}

void TileTool::Update( Canvas& canvas )
{
	AbstractTool::Update( canvas );
	ExamineMousePosition();
}

void TileTool::ClearMouseTextureData()
{
	m_pMouseTile->sprite = SpriteComponent();
}

void TileTool::LoadSpriteTextureData( const std::string& textureName )
{
	m_pMouseTile->sprite = SpriteComponent{
		.width = m_MouseRect.x,
		.height = m_MouseRect.y,
		.color = Color{ 255, 255, 255, 255 },
		.start_x = 0,
		.start_y = 0,
		.layer = 0,
		.texture_name = textureName };
	auto& pTexture = MAIN_REGISTRY().GetAssetManager().GetTexture( textureName );
	assert( pTexture.getID() != 0 && "Texture must exists" );
	m_pMouseTile->sprite.generate_uvs( pTexture.getWidth(), pTexture.getHeight() );
}

const std::string& TileTool::GetSpriteTexture() const
{
	if ( !m_pMouseTile )
	{
		return std::string{ "" };
	}
	return m_pMouseTile->sprite.texture_name;
}

void TileTool::SetBoxCollider( int width, int height, glm::vec2& offset )
{
	m_pMouseTile->boxCollider.width = width;
	m_pMouseTile->boxCollider.height = height;
	m_pMouseTile->boxCollider.offset = offset;
}

void TileTool::SetSpriteLayer( int layer )
{
	m_pMouseTile->sprite.layer = layer;
}

void TileTool::SetScale( const glm::vec2& scale )
{
	m_pMouseTile->transform.scale = scale;
}

void TileTool::SetAnimation( int numFrames, int frameRate, bool bVertical, bool bLooped, int frameOffset )
{
	m_pMouseTile->animation.numFrames = numFrames;
	m_pMouseTile->animation.frameRate = frameRate;
	m_pMouseTile->animation.bVertical = bVertical;
	m_pMouseTile->animation.bLooped = bLooped;
	m_pMouseTile->animation.frameOffset = frameOffset;
}

void TileTool::SetSpriteUVs( int startX, int startY )
{
	m_pMouseTile->sprite.start_x = startX;
	m_pMouseTile->sprite.start_y = startY;
	m_pMouseTile->sprite.uvs.u = startX * m_pMouseTile->sprite.uvs.uv_width;
	m_pMouseTile->sprite.uvs.v = startY * m_pMouseTile->sprite.uvs.uv_height;
}

void TileTool::SetSpriteRect( const glm::vec2& spriteRect )
{
	if ( m_pMouseTile->sprite.texture_name.empty() )
		return;

	m_MouseRect = spriteRect;
	auto& sprite = m_pMouseTile->sprite;
	sprite.width = m_MouseRect.x;
	sprite.height = m_MouseRect.y;

	auto& pTexture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
	assert( pTexture.getID() != 0 && "Texture Must exist." );
	sprite.generate_uvs( pTexture.getWidth(), pTexture.getHeight() );
}

void TileTool::SetCollider( bool bCollider )
{
	m_pMouseTile->bCollider = bCollider;
}

void TileTool::SetCircle( bool bCircle )
{
	m_pMouseTile->bCircle = bCircle;
}

void TileTool::SetAnimation( bool bAnimation )
{
	m_pMouseTile->bAnimation = bAnimation;
}

void TileTool::SetPhysics( bool bPhysics )
{
	m_pMouseTile->bPhysics = bPhysics;
}

const bool TileTool::SpriteValid() const
{
	return !m_pMouseTile->sprite.texture_name.empty();
}

const bool TileTool::CanDrawOrCreate() const
{
	return IsActivated() && !OutOfBounds() && IsOverTilemapWindow() && SpriteValid();
}
