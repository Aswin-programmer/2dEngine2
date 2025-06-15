#pragma once

#include "Vertex.h"
#include "Font.h"

// Batch and the SpriteGlyph for the sprite rendering.
struct Batch
{
	GLuint numIndices{ 0 }, offset{ 0 }, textureID{ 0 };
};

struct SpriteGlyph
{
	Vertex topLeft, bottomLeft, topRight, bottomRight;
	int layer;
	GLuint textureID;
};

// LineBatch and LineGlyph for the line rendering
struct LineBatch
{
	GLuint offset{ 2 }, numVertices{ 0 };
	float lineWidth{ 1.0f };
};

struct LineGlyph
{
	Vertex p1, p2;
	float lineWidth;
};

// RectBatch and RectGlyph for the rectangle rendering
struct RectBatch
{
	GLuint numIndices{ 0 }, offset{ 0 };
};

struct RectGlyph
{
	Vertex topLeft, bottomLeft, topRight, bottomRight;
};

// RectBatch and CircleGlyph for the circle rendering
struct CircleGlyph
{
	CircleVertex topLeft, bottomLeft, topRight, bottomRight;
};

// TextBatch and TextGlyph for the text rendering.
struct TextBatch
{
	GLuint offset{ 0 }, numVertices{ 0 }, fontAtlasID{ 0 };
};

struct TextGlyph
{
	std::string textStr{ "" };
	glm::vec2 position{ 0.f };
	Color color{ 255, 255, 255, 255 };
	glm::mat4 model{ 1.f };
	std::shared_ptr<Font> font{ nullptr };
	float wrap{ 0.f };
	int padding{ 0 };
};
