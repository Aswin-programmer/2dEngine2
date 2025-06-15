#pragma once

#include "IDisplay.h"

class Camera2D;

class TilemapDisplay : public IDisplay
{
private:
	std::unique_ptr<Camera2D> m_pTilemapCam;

private:
	void RenderTilemap();
	void LoadNewScene();
	void PanZoomCamera( const glm::vec2& mousePos );

public:
	TilemapDisplay();
	virtual ~TilemapDisplay() = default;

	virtual void Draw() override;
	virtual void Update() override;
};
