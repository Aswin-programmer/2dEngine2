#include "pch.h"

#include "RENDERER/TEXTURE/Texture.h"

#include "LOGGER/log.h"

#include <stb_image.h>

Texture::Texture()
	: filePath( nullptr ),
	  texture1( 0 ),
	  width( 0 ),
	  height( 0 ),
	  m_bTileSetTexture( false ),
	  m_bEditorTexture( false )
{
}

Texture::Texture( TEXTURETYPE::FILEPATH, const char* filePath ) : Texture()
{
	InitTextureFromFilePath( filePath );
}

Texture::Texture( TEXTURETYPE::FRAMEBUFFER, const int& width, const int& height ) : Texture()
{
	InitTextureFromFrameBuffer( width, height );
}

Texture::Texture( TEXTURETYPE::TEXTURE_FROM_MEMORY, const unsigned char* fileData, int fileSize ) : Texture()
{
	InitTextureFromMemory( fileData, fileSize );
}

Texture::Texture( TEXTURETYPE::FILEPATH, TEXTURECATEGORY::TILESETTEXTURE, const char* filePath ) : Texture()
{
	m_bTileSetTexture = true;
	InitTextureFromFilePath( filePath );
}

Texture::Texture( TEXTURETYPE::TEXTURE_FROM_MEMORY, TEXTURECATEGORY::EDITORTEXTURE, const unsigned char* fileData, int fileSize )
{
	m_bEditorTexture = true;
	InitTextureFromMemory( fileData, fileSize );
}

void Texture::InitTextureFromFilePath( const char* file_path )
{
	filePath = file_path;
	glGenTextures( 1, &texture1 );
	glBindTexture( GL_TEXTURE_2D, texture1 );

	// set the texture wrapping parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// set texture filtering parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	// load image, create texture and generate mipmaps
	int nrChannels;
	// stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load( filePath, &width, &height, &nrChannels, 0 );
	if ( data )
	{
		if ( nrChannels == 4 )
		{
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
			glGenerateMipmap( GL_TEXTURE_2D );
		}
		else if ( nrChannels == 3 )
		{
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
			glGenerateMipmap( GL_TEXTURE_2D );
		}
		else
		{
			std::cout << "UNKNOWN CHANNEL FOR TEXTURE LOADING!" << std::endl;
			LOG_ERROR( "UNKNOWN CHANNEL FOR TEXTURE LOADING! :{0}", texture1 );
		}
	}
	else
	{
		std::cout << "Failed to load texture: " << filePath << std::endl; // One error has to be corrected. The asset manager will not be indicated incase of this error.
		LOG_ERROR( "FAILED TO LOAD TEXTURE :{0}", filePath );
		width = 0;
		height = 0;
	}
	stbi_image_free( data );
}

void Texture::InitTextureFromFrameBuffer( const int& frameBufferWidth, const int& frameBufferHeight )
{
	width = frameBufferWidth;
	height = frameBufferHeight;

	glGenTextures( 1, &texture1 );
	glBindTexture( GL_TEXTURE_2D, texture1 );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}

void Texture::InitTextureFromMemory( const unsigned char* fileData, int fileSize )
{
	glGenTextures( 1, &texture1 );
	glBindTexture( GL_TEXTURE_2D, texture1 );

	// Set texture parameters (same as before)...
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	// Use STB Image to decode from memory
	int width, height, nrChannels;
	// stbi_set_flip_vertically_on_load(true); // Optional
	unsigned char* pixelData = stbi_load_from_memory(
		fileData, // Pointer to compressed image data
		fileSize, // Size of the compressed data buffer
		&width,
		&height,
		&nrChannels,
		0 );

	if ( pixelData )
	{
		// Determine format based on channel count
		GLenum format = GL_RGB;
		if ( nrChannels == 4 )
			format = GL_RGBA;
		else if ( nrChannels == 1 )
			format = GL_RED;

		glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixelData );
		glGenerateMipmap( GL_TEXTURE_2D );

		this->width = width;
		this->height = height;

		stbi_image_free( pixelData );
	}
	else
	{
		std::cerr << "Failed to decode texture from memory!\n";
		LOG_ERROR( "STB failed to decode image from memory" );
	}
}

Texture::~Texture()
{
	if ( texture1 != 0 )
	{
		glDeleteTextures( 1, &texture1 );
	}
}

void Texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, texture1 );
}

void Texture::unbind()
{
	glBindTexture( GL_TEXTURE_2D, 0 );
}
