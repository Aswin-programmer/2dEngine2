#include "pch.h"

#include "RENDERER/BUFFERS/FrameBuffer.h"

#include "LOGGER/log.h"
#include "RENDERER/TEXTURE/TextureLoader.h"

bool FrameBuffer::Initialize()
{
	glGenFramebuffers( 1, &m_FboID );
	glBindFramebuffer( GL_FRAMEBUFFER, m_FboID );
	if ( m_FboID == 0 )
	{
		// Handle the error, perhaps log it
		LOG_ERROR( "Failed to generate framebuffer object" );
	}

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getID(), 0 );

	if ( m_bUseRbo )
	{
		glGenRenderbuffers( 1, &m_RboID );
		glBindRenderbuffer( GL_RENDERBUFFER, m_RboID );
		if ( m_RboID == 0 )
		{
			// Handle the error, perhaps log it
			LOG_ERROR( "Failed to generate framebuffer object" );
		}

		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_Width, m_Height );

		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RboID );
	}

	if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	{
		assert( false && "Failed to create an OpenGL Framebuffer!" );

		std::string error = std::to_string( glGetError() );
		LOG_ERROR( "Failed to create an OpenGL framebuffer: {}", error );
		return false;
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	return true;
}

void FrameBuffer::CleanUp()
{
	glDeleteFramebuffers( 1, &m_FboID );
	if ( m_bUseRbo )
		glDeleteRenderbuffers( 1, &m_RboID );

	if ( m_pTexture )
	{
		auto textureID = m_pTexture->getID();
		glDeleteTextures( 1, &textureID );
	}
}

FrameBuffer::FrameBuffer()
	: FrameBuffer( 640, 480, false )
{
}

FrameBuffer::FrameBuffer( int width, int height, bool bUseRbo )
	: m_FboID{ 0 }, m_RboID{ 0 }, m_pTexture{ nullptr }, m_Width{ width }, m_Height{ height }, m_bShouldResize{ false }, m_bUseRbo{ bUseRbo }
{
	m_pTexture = std::move( TextureLoader::createFrameBufferTexture( width, height ) );

	if ( !m_pTexture || !Initialize() )
	{
		assert( false && "Failed to create Framebuffer!" );
		LOG_ERROR( "Framebuffer creation failed" );
	}
}

FrameBuffer::~FrameBuffer()
{
	CleanUp();
}

void FrameBuffer::Bind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, m_FboID );
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void FrameBuffer::Resize( int width, int height )
{
	m_Width = width;
	m_Height = height;
	m_bShouldResize = true;
}

void FrameBuffer::CheckResize()
{
	if ( !m_bShouldResize )
		return;

	CleanUp();
	m_pTexture.reset();
	m_pTexture = std::move( TextureLoader::createFrameBufferTexture( m_Width, m_Height ) );

	assert( m_pTexture && "New Texture cannot be nullptr!" );

	Initialize();
	m_bShouldResize = false;
}
