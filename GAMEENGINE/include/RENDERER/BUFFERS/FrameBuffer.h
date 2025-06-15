#pragma once

#include "RENDERER/TEXTURE/Texture.h"

class FrameBuffer
{
private:
	GLuint m_FboID, m_RboID;
	std::shared_ptr<Texture> m_pTexture;
	int m_Width, m_Height;
	bool m_bShouldResize, m_bUseRbo;

private:
	bool Initialize();
	void CleanUp();

public:
	FrameBuffer();
	FrameBuffer( int width, int height, bool bUseRbo );
	~FrameBuffer();

	void Bind();
	void Unbind();

	// Resizing
	void Resize( int width, int height );
	void CheckResize();

	inline const GLuint GetID() const { return m_FboID; }
	inline const GLuint GetTextureID() const { return m_pTexture ? m_pTexture->getID() : 0; }
	inline const int Width() const { return m_Width; }
	inline const int Height() const { return m_Height; }
};
