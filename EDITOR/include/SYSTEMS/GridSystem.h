#pragma once

class RectBatchRenderer;
class Camera2D;
class SceneObject;

class GridSystem
{
private:
	std::unique_ptr<RectBatchRenderer> m_pBatchRenderer;

public:
	GridSystem();
	~GridSystem() = default;

	void Update( SceneObject& currentScene, Camera2D& camera );
};
