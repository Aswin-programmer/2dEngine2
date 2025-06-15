#pragma once

#include "Batcher.h"
#include "RENDERER/ESSENTIALS/BatchTypes.h"

struct Line;

class LineBatchRenderer : public Batcher<LineBatch, LineGlyph>
{
private:
	virtual void GenerateBatches() override;
	void Initialize();

public:
	LineBatchRenderer();
	virtual ~LineBatchRenderer() = default;

	virtual void End() override;
	virtual void Render() override;

	void AddLine( const Line& line );
};
