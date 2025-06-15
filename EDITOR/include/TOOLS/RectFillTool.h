#pragma once

#include "TileTool.h"

constexpr int PRVIEW_LAYER = 10;

class RectBatchRenderer;
struct Rect;

class RectFillTool : public TileTool
{
private:
	std::unique_ptr<RectBatchRenderer> m_pShapeRenderer;
	std::unique_ptr<Rect> m_pTileFillRect;
	glm::vec2 m_StartPressPos;

private:
	void CreateTiles();
	void RemoveTiles();
	void DrawPreview( int dx, int dy );
	void ResetTile();

public:
	RectFillTool();
	virtual ~RectFillTool();

	virtual void Create() override;
	virtual void Draw() override;
};
