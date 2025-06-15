#pragma once

#include "RENDERER/ESSENTIALS/Vertex.h"

struct TextComponent
{
	std::string sFontName{ "pixel" }, sTextStr{ "" };
	int padding{ 0 };
	float wrap{ -1.f };
	Color color{ 255, 255, 255, 255 };
	bool bHidden{ false };

	static void CreateLuaTextBindings( sol::state& lua );
};
