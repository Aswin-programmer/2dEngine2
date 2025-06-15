#pragma once

constexpr size_t NUM_SPRITE_VERTICES = 4;
constexpr size_t NUM_SPRITE_INDICES = 6;
constexpr size_t MAX_SPRITES = 5000;
constexpr size_t MAX_INDICES = MAX_SPRITES * NUM_SPRITE_INDICES;
constexpr size_t MAX_VERTICES = MAX_SPRITES * NUM_SPRITE_VERTICES;

template <typename TBatch, typename TGlyph>
class Batcher
{
private:
	GLuint m_VAO, m_VBO, m_IBO;
	bool m_bUseIBO;

private:
	void Initialize();

protected:
	int checkMaxNumSprites = 0;
	std::vector<std::shared_ptr<TGlyph>> m_Glyphs;
	std::vector<std::shared_ptr<TBatch>> m_Batches;

protected:
	void SetVertexAttribute( GLuint layoutPosition, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset, GLboolean normalized = GL_FALSE );

	inline GLuint GetVBO() const { return m_VBO; }
	inline GLuint GetIBO() const { return m_IBO; }
	inline void EnableVAO() const { glBindVertexArray( m_VAO ); }
	inline void DisableVAO() const { glBindVertexArray( 0 ); }

	virtual void GenerateBatches() = 0;

public:
	Batcher();
	Batcher( bool bUseIBO );
	virtual ~Batcher();

	/*
	 * @brief Clears the current batches and sprites making
	 * it ready to start new batches.
	 */
	void Begin();

	virtual void End() = 0;
	virtual void Render() = 0;
};
