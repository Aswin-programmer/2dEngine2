#pragma once

class RectBatchRenderer;
class CircleBatchRenderer;
class Registry;
class Camera2D;

class RenderShapeSystem
{
private:
	std::unique_ptr<RectBatchRenderer> m_pRectRenderer;
	std::unique_ptr<CircleBatchRenderer> m_pCircleRenderer;

public:
	RenderShapeSystem();
	~RenderShapeSystem() = default;

	void Update( Registry& registry, Camera2D& camera );
};
