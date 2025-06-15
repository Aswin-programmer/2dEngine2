#pragma once

class SpriteBatchRenderer;
class TextBatchRenderer;
class Registry;
class Camera2D;

class RenderUISystem
{
private:
	std::unique_ptr<SpriteBatchRenderer> m_pSpriteRenderer;
	std::unique_ptr<TextBatchRenderer> m_pTextRenderer;
	std::unique_ptr<Camera2D> m_pCamera2D;

public:
	RenderUISystem();
	~RenderUISystem() = default;

	void Update( Registry& registry, Camera2D& camera );
};
