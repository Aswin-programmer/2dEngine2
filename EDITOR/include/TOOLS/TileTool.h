#pragma once

#include "AbstractTool.h"

constexpr int MOUSE_SPRITE_LAYER = 10;

class SpriteBatchRenderer;
class Entity;

class TileTool : public AbstractTool
{
private:
	glm::vec2 m_MouseRect;
	glm::vec2 m_GridCoords;

	bool m_bGridSnap;

protected:
	std::shared_ptr<SpriteBatchRenderer> m_pBatchRenderer;
	std::shared_ptr<struct Tile> m_pMouseTile;

protected:
	uint32_t CheckForTile( const glm::vec2& position );

	Entity CreateEntity();
	Entity CreateEntity( uint32_t id );

	void DrawMouseSprite();
	virtual void ExamineMousePosition() override;

public:
	TileTool();
	virtual ~TileTool() = default;

	virtual void Create() = 0;
	virtual void Draw() = 0;

	virtual void Update( Canvas& canvas ) override;

	void ClearMouseTextureData();
	void LoadSpriteTextureData( const std::string& textureName );
	const std::string& GetSpriteTexture() const;

	void SetBoxCollider( int width, int height, glm::vec2& offset );
	void SetSpriteLayer( int layer );
	void SetScale( const glm::vec2& scale );
	void SetAnimation( int numFrames, int frameRate, bool bVertical, bool bLooped, int frameOffset );
	void SetSpriteUVs( int startX, int startY );

	// TODO: Set physics props
	void SetSpriteRect( const glm::vec2& spriteRect );

	void SetCollider( bool bCollider );
	void SetCircle( bool bCircle );
	void SetAnimation( bool bAnimation );
	void SetPhysics( bool bPhysics );

	const bool SpriteValid() const;
	const bool CanDrawOrCreate() const;

	inline void EnableGridSnap() { m_bGridSnap = true; }
	inline void DisableGridSnap() { m_bGridSnap = false; }
};
