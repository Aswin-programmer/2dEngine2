#pragma once

#include "TileTool.h"

class CreateTileTool : public TileTool
{
private:
	void CreateNewTile();
	void RemoveTile();

public:
	CreateTileTool();
	virtual ~CreateTileTool();

	virtual void Create() override;
	virtual void Draw() override;
};
