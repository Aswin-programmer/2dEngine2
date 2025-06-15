#pragma once

#include "IDisplay.h"

class MenuDisplay : public IDisplay
{
private:
public:
	MenuDisplay() = default;
	virtual ~MenuDisplay() = default;

	virtual void Draw() override;
	virtual void Update() override;
};
