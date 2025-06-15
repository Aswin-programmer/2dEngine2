#pragma once

class Registry;

struct RendererBinder
{
	static void CreateRenderingBind( sol::state& lua, Registry& registry );
};
