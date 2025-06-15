#include "pch.h"

#include "RENDERER/CORE/Batcher.h"

#include "RENDERER/ESSENTIALS/BatchTypes.h"
// #include "RENDERER/ESSENTIALS/Vertex.h"

template class Batcher<Batch, SpriteGlyph>;
template class Batcher<LineBatch, LineGlyph>;
template class Batcher<RectBatch, RectGlyph>;
template class Batcher<RectBatch, CircleGlyph>;
template class Batcher<TextBatch, TextGlyph>;

template <typename TBatch, typename TGlyph>
Batcher<TBatch, TGlyph>::Batcher()
	: Batcher( true )
{
}

template <typename TBatch, typename TGlyph>
Batcher<TBatch, TGlyph>::Batcher( bool bUseIBO )
	: m_VAO{ 0 }, m_VBO{ 0 }, m_IBO{ 0 }, m_Glyphs{}, m_Batches{}, m_bUseIBO{ bUseIBO }
{
	Initialize();
}

template <typename TBatch, typename TGlyph>
Batcher<TBatch, TGlyph>::~Batcher()
{
	if ( m_VAO )
		glDeleteVertexArrays( 1, &m_VAO );
	if ( m_VBO )
		glDeleteBuffers( 1, &m_VBO );
	if ( m_IBO )
		glDeleteBuffers( 1, &m_IBO );
}

template <typename TBatch, typename TGlyph>
void Batcher<TBatch, TGlyph>::Begin()
{
	checkMaxNumSprites = 0;
	m_Glyphs.clear();
	m_Batches.clear();
}

template <typename TBatch, typename TGlyph>
void Batcher<TBatch, TGlyph>::Initialize()
{
	// Let's generate the VAO
	glGenVertexArrays( 1, &m_VAO );
	// Generate the VBO
	glGenBuffers( 1, &m_VBO );
	// Bind the VAO and VBO
	glBindVertexArray( m_VAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
	glBufferData( GL_ARRAY_BUFFER, MAX_VERTICES * sizeof( Vertex ), nullptr, GL_DYNAMIC_DRAW );

	if ( !m_bUseIBO )
	{
		glBindVertexArray( 0 );
		return;
	}

	GLuint offset{ 0 };
	GLuint indices[NUM_SPRITE_INDICES]{ 0, 1, 2, 2, 3, 0 };
	auto indicesArr = std::make_unique<GLuint[]>( MAX_INDICES );
	for ( size_t i = 0; i < MAX_INDICES; i += NUM_SPRITE_INDICES )
	{
		for ( size_t j = 0; j < NUM_SPRITE_INDICES; j++ )
			indicesArr[i + j] = indices[j] + offset;
		offset += NUM_SPRITE_VERTICES;
	}
	// Generate the index buffer
	glGenBuffers( 1, &m_IBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLuint ) * MAX_INDICES, indicesArr.get(), GL_DYNAMIC_DRAW );
	glBindVertexArray( 0 );
}

template <typename TBatch, typename TGlyph>
void Batcher<TBatch, TGlyph>::SetVertexAttribute( GLuint layoutPosition, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset, GLboolean normalized )
{
	glBindVertexArray( m_VAO );
	glVertexAttribPointer( layoutPosition, numComponents, type, normalized, stride, offset );
	glEnableVertexAttribArray( layoutPosition );
	glBindVertexArray( 0 );
}
