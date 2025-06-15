#pragma once

#include "Texture.h"

class TextureLoader
{
public:
	static std::shared_ptr<Texture> createTexture( const char* filePath )
	{
		return std::make_shared<Texture>( TEXTURETYPE::FILEPATH{}, filePath );
	}

	static std::shared_ptr<Texture> createFrameBufferTexture( int width, int height )
	{
		return std::make_shared<Texture>( TEXTURETYPE::FRAMEBUFFER{}, width, height );
	}

	static std::shared_ptr<Texture> createTextureFromMemory( const unsigned char* imageData, int fileSize )
	{
		return std::make_shared<Texture>( TEXTURETYPE::TEXTURE_FROM_MEMORY{}, imageData, fileSize );
	}

	static std::shared_ptr<Texture> createEditorTextureFromMemory( const unsigned char* imageData, int fileSize )
	{
		return std::make_shared<Texture>( TEXTURETYPE::TEXTURE_FROM_MEMORY{}, TEXTURECATEGORY::EDITORTEXTURE{}, imageData, fileSize );
	}

	static std::shared_ptr<Texture> createTilesetTextureTexture( const char* filePath )
	{
		return std::make_shared<Texture>( TEXTURETYPE::FILEPATH{}, TEXTURECATEGORY::TILESETTEXTURE{}, filePath );
	}
};
