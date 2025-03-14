#include "ScriptingSystem.h"

ScriptingSystem::ScriptingSystem(Registry& registry)
	:
	m_Registry(registry), m_bMainLoaded{ false }
{
}

bool ScriptingSystem::LoadMainScript(sol::state& lua)
{
	try
	{
		auto result = lua.safe_script_file("ASSETS/SCRIPTS/main.lua");
	}
	catch (const sol::error& err)
	{
		LOG_ERROR("Error loading the main lua script: {0}", err.what());
		return false;
	}
	sol::table main_lua = lua["main"];
	sol::optional<sol::table> bUpdateExists = main_lua[1];
	if (bUpdateExists == sol::nullopt)
	{
		LOG_ERROR("There is no update function in main.lua");
		return false;
	}
	sol::table update_script = main_lua[1];
	sol::function update = update_script["update"];

	sol::optional<sol::table> bRenderExists = main_lua[2];
	if (bRenderExists == sol::nullopt)
	{
		LOG_ERROR("There is no render function in main.lua");
		return false;
	}
	sol::table render_script = main_lua[2];
	sol::function render = render_script["render"];
	Entity mainLuaScript{ m_Registry, "main_script", "" };
	mainLuaScript.AddComponent<ScriptComponent>(
		ScriptComponent{
			.update = update,
			.render = render
		}
	);
	m_bMainLoaded = true;
	return true;
}

void ScriptingSystem::Update()
{
	if (!m_bMainLoaded)
	{
		LOG_ERROR("Main lua script has not been loaded!");
		return;
	}
	auto view = m_Registry.GetRegistry().view<ScriptComponent>();
	for (const auto& entity : view)
	{
		Entity ent{ m_Registry, entity };
		if (ent.GetName() != "main_script")
			continue;
		auto& script = ent.GetComponent<ScriptComponent>();
		auto error = script.update(entity);
		if (!error.valid())
		{
			sol::error err = error;
			LOG_ERROR("Error running the Update script: {0}", err.what());
		}
	}
}

void ScriptingSystem::Render()
{
	if (!m_bMainLoaded)
	{
		LOG_ERROR("Main lua script has not been loaded!");
		return;
	}
	auto view = m_Registry.GetRegistry().view<ScriptComponent>();
	for (const auto& entity : view)
	{
		Entity ent{ m_Registry, entity };
		if (ent.GetName() != "main_script")
			continue;
		auto& script = ent.GetComponent<ScriptComponent>();
		auto error = script.render(entity);
		if (!error.valid())
		{
			sol::error err = error;
			LOG_ERROR("Error running the Render script: {0}", err.what());
		}
	}
}

auto create_timer = [](sol::state& lua) {
	lua.new_usertype<Timer>(
		"Timer",
		sol::call_constructor,
		sol::factories([]() {return Timer{}; }),
		"start", &Timer::Start,
		"stop", &Timer::Stop,
		"pause", &Timer::Pause,
		"resume", &Timer::Resume,
		"is_paused", &Timer::IsPaused,
		"is_running", &Timer::IsRunning,
		"elapsed_ms", &Timer::ElapsedMS,
		"elapsed_sec", &Timer::ElapsedSec,
		"restart", [](Timer& timer) {
			if (timer.IsRunning())
				timer.Stop();
			timer.Start();
		}
	);
};

void ScriptingSystem::RegisterLuaBindings(sol::state& lua, Registry& registry)
{
	Entity::CreateLuaEntityBind(lua, registry);
	Registry::CreateLuaRegistryBind(lua, registry);
	GLMBindings::CreateGLMBindings(lua);
	InputManager::CreateLuaInputBindings(lua);
	AssetManager::CreateLuaAssetManager(lua, registry);

	create_timer(lua);
	

	TransformComponent::CreateLuaTransformBind(lua);
	SpriteComponent::CreateSpriteLuaBind(lua, registry);
	AnimationComponent::CreateAnimationLuaBind(lua);
	BoxColliderComponent::CreateLuaBoxColliderBind(lua);
	CircleColliderComponent::CreateLuaCircleColliderBind(lua);
	SoundEmitter::CreateLuaSoundEmitterBind(lua, registry);
	SoundListener::CreateLuaSoundListenerBind(lua);

	Entity::RegisterMetaComponent<TransformComponent>();
	Entity::RegisterMetaComponent<SpriteComponent>();
	Entity::RegisterMetaComponent<AnimationComponent>();
	Entity::RegisterMetaComponent<BoxColliderComponent>();
	Entity::RegisterMetaComponent<CircleColliderComponent>();
	Entity::RegisterMetaComponent<SoundEmitter>();
	Entity::RegisterMetaComponent<SoundListener>();

	Registry::RegisterMetaComponent<TransformComponent>();
	Registry::RegisterMetaComponent<SpriteComponent>();
	Registry::RegisterMetaComponent<AnimationComponent>();
	Registry::RegisterMetaComponent<BoxColliderComponent>();
	Registry::RegisterMetaComponent<CircleColliderComponent>();
	Registry::RegisterMetaComponent<SoundEmitter>();
	Registry::RegisterMetaComponent<SoundListener>();
}

void ScriptingSystem::RegisterLuaFunctions(sol::state& lua)
{
	lua.set_function(
		"run_script", [&](const std::string& path)
		{
			try
			{
				lua.safe_script_file(path);
			}
			catch (const sol::error& error)
			{
				LOG_ERROR("Error loading Lua Script: {}", error.what());
				return false;
			}
			return true;
		}
	);
}
