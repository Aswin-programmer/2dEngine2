#pragma once

#include "../ECS/Registry.h"
#include "../ECS/COMPONENTS/ScriptComponent.h"
#include "../ECS/COMPONENTS/TransformComponent.h"
#include "../ECS/COMPONENTS/SpriteComponent.h"
#include "../ECS/Components/AnimationComponent.h"
#include "../ECS/Components/BoxColliderComponent.h"
#include "../ECS/Components/CircleColliderComponent.h"
#include "../ECS/COMPONENTS/SoundEmitter.h"
#include "../ECS/COMPONENTS/SoundListener.h"
#include "../ECS/Entity.h"
#include "../LOGGER/log.h"
#include "../SCRIPTING/GLMLuaBindings.h"
#include "../SCRIPTING/InputManager.h"
#include "../SCRIPTING/SoundBinding.h"
#include "../RESOURCES/AssetManager.h"
#include "../UTILITIES/Timer.h"
#include "../SOUNDS/AudioEvent.h"
#include "SoundSystem.h"

#include <sol/sol.hpp>

class ScriptingSystem
{
public:
	ScriptingSystem(Registry& registry);
	~ScriptingSystem() = default;
	bool LoadMainScript(sol::state& lua);
	void Update();
	void Render();

	static void RegisterLuaBindings(sol::state& lua, Registry& registry);

	static void RegisterLuaFunctions(sol::state& lua);


private:
	Registry& m_Registry;
	bool m_bMainLoaded;
};
