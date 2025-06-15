#pragma once

struct TEXTURETYPE
{
	struct FILEPATH
	{
	};
	struct FRAMEBUFFER
	{
	};
	struct TEXTURE_FROM_MEMORY
	{
	};
};

struct TEXTURECATEGORY
{
	struct TILESETTEXTURE
	{
	};
	struct EDITORTEXTURE
	{
	};
};

class Texture
{
public:
	Texture();
	// Factory methods to get the texture:
	Texture( TEXTURETYPE::FILEPATH, const char* filePath );
	Texture( TEXTURETYPE::FRAMEBUFFER, const int& width, const int& height );
	Texture( TEXTURETYPE::TEXTURE_FROM_MEMORY, const unsigned char* fileData, int fileSize );
	Texture( TEXTURETYPE::FILEPATH, TEXTURECATEGORY::TILESETTEXTURE, const char* filePath );
	Texture( TEXTURETYPE::TEXTURE_FROM_MEMORY, TEXTURECATEGORY::EDITORTEXTURE, const unsigned char* fileData, int fileSize );

	void InitTextureFromFilePath( const char* filePath );
	void InitTextureFromFrameBuffer( const int& width, const int& height ); // For the Frame Buffer.
	void InitTextureFromMemory( const unsigned char* fileData, int fileSize );
	~Texture();
	void bind();
	void unbind();
	unsigned int getID() const { return texture1; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	const bool IsTileset() const { return m_bTileSetTexture; }
	void SetIsTileset( bool bIsTileset ) { m_bTileSetTexture = bIsTileset; }
	const bool IsEditorTexture() const { return m_bEditorTexture; }
	void SetIsEditorTexture( bool bIsEditorTexture ) { m_bEditorTexture = bIsEditorTexture; }

private:
	const char* filePath;
	unsigned int texture1;
	int width;
	int height;
	bool m_bTileSetTexture, m_bEditorTexture;
};
