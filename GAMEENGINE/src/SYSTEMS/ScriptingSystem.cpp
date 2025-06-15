#include "pch.h"

#include "SYSTEMS/ScriptingSystem.h"

#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include "ECS/MainRegistry.h"
#include "ECS/Components/AnimationComponent.h"
#include "ECS/Components/BoxColliderComponent.h"
#include "ECS/Components/CircleColliderComponent.h"
#include "ECS/COMPONENTS/OpenALSoundComponent.h"
#include "ECS/Components/PhysicsComponent.h"
#include "ECS/COMPONENTS/RigidBodyComponent.h"
#include "ECS/COMPONENTS/ScriptComponent.h"
#include "ECS/COMPONENTS/SpriteComponent.h"
#include "ECS/COMPONENTS/TextComponent.h"
#include "ECS/COMPONENTS/TransformComponent.h"
#include "LOGGER/log.h"
#include "LOGGER/LuaLogBinding.h"
#include "RENDERER/CORE/FollowCamera.h"
#include "RENDERER/ESSENTIALS/Font.h"
#include "RESOURCES/AssetManager.h"
#include "SCRIPTING/ContactListenerBind.h"
#include "SCRIPTING/GLMLuaBindings.h"
#include "SCRIPTING/InputManager.h"
#include "SCRIPTING/RendererBindings.h"
#include "SCRIPTING/SensorListenerBind.h"
#include "SCRIPTING/UserDataBinding.h"
#include "STATES/State.h"
#include "STATES/StateStack.h"
#include "STATES/StateMachine.h"
#include "STATES/StateSubMachine.h"
#include "UTILITIES/RandomGenerator.h"
#include "UTILITIES/Timer.h"

ScriptingSystem::ScriptingSystem( Registry& registry )
	: m_Registry( registry ), m_bMainLoaded{ false }
{
}

bool ScriptingSystem::LoadMainScript( sol::state& lua )
{
	try
	{
		auto result = lua.safe_script_file( "assets/SCRIPTS/main.lua" );
	}
	catch ( const sol::error& err )
	{
		LOG_ERROR( "Error loading the main lua script: {0}", err.what() );
		return false;
	}
	sol::table main_lua = lua["main"];
	sol::optional<sol::table> bUpdateExists = main_lua[1];
	if ( bUpdateExists == sol::nullopt )
	{
		LOG_ERROR( "There is no update function in main.lua" );
		return false;
	}
	sol::table update_script = main_lua[1];
	sol::function update = update_script["update"];

	sol::optional<sol::table> bRenderExists = main_lua[2];
	if ( bRenderExists == sol::nullopt )
	{
		LOG_ERROR( "There is no render function in main.lua" );
		return false;
	}
	sol::table render_script = main_lua[2];
	sol::function render = render_script["render"];
	Entity mainLuaScript{ m_Registry, "main_script", "" };
	mainLuaScript.AddComponent<ScriptComponent>(
		ScriptComponent{
			.update = update,
			.render = render } );
	m_bMainLoaded = true;
	return true;
}

void ScriptingSystem::Update()
{
	if ( !m_bMainLoaded )
	{
		LOG_ERROR( "Main lua script has not been loaded!" );
		return;
	}
	auto view = m_Registry.GetRegistry().view<ScriptComponent>();
	for ( const auto& entity : view )
	{
		Entity ent{ m_Registry, entity };
		if ( ent.GetName() != "main_script" )
			continue;
		auto& script = ent.GetComponent<ScriptComponent>();
		auto error = script.update( entity );
		if ( !error.valid() )
		{
			sol::error err = error;
			LOG_ERROR( "Error running the Update script: {0}", err.what() );
		}
	}

	auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();
	if ( lua )
		lua->collect_garbage();
}

void ScriptingSystem::Render()
{
	if ( !m_bMainLoaded )
	{
		LOG_ERROR( "Main lua script has not been loaded!" );
		return;
	}
	auto view = m_Registry.GetRegistry().view<ScriptComponent>();
	for ( const auto& entity : view )
	{
		Entity ent{ m_Registry, entity };
		if ( ent.GetName() != "main_script" )
			continue;
		auto& script = ent.GetComponent<ScriptComponent>();
		auto error = script.render( entity );
		if ( !error.valid() )
		{
			sol::error err = error;
			LOG_ERROR( "Error running the Render script: {0}", err.what() );
		}
	}

	auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();
	if ( lua )
		lua->collect_garbage();
}

auto create_timer = []( sol::state& lua ) {
	lua.new_usertype<Timer>(
		"Timer",
		sol::call_constructor,
		sol::factories( []() { return Timer{}; } ),
		"start", &Timer::Start,
		"stop", &Timer::Stop,
		"pause", &Timer::Pause,
		"resume", &Timer::Resume,
		"is_paused", &Timer::IsPaused,
		"is_running", &Timer::IsRunning,
		"elapsed_ms", &Timer::ElapsedMS,
		"elapsed_sec", &Timer::ElapsedSec,
		"restart", []( Timer& timer ) {
			if ( timer.IsRunning() )
				timer.Stop();
			timer.Start();
		} );
};

void ScriptingSystem::RegisterLuaBindings( sol::state& lua, Registry& registry )
{
	LuaLogBinding::RegisterLuaLogBinding( lua );

	Entity::CreateLuaEntityBind( lua, registry );
	Registry::CreateLuaRegistryBind( lua, registry );
	GLMBindings::CreateGLMBindings( lua );
	InputManager::CreateLuaInputBindings( lua, registry );
	AssetManager::CreateLuaAssetManager( lua, registry );
	// SoundSystem::CreateLuaSoundSystemBind(lua, registry);

	create_timer( lua );

	TransformComponent::CreateLuaTransformBind( lua );
	SpriteComponent::CreateSpriteLuaBind( lua, registry );
	AnimationComponent::CreateAnimationLuaBind( lua );
	BoxColliderComponent::CreateLuaBoxColliderBind( lua );
	CircleColliderComponent::CreateLuaCircleColliderBind( lua );
	// SoundEmitter::CreateLuaSoundEmitterBind(lua, registry);
	// SoundListener::CreateLuaSoundListenerBind(lua);
	RendererBinder::CreateRenderingBind( lua, registry );
	PhysicsComponent::CreatePhysicsLuaBind( lua, registry );
	TextComponent::CreateLuaTextBindings( lua );
	RigidBodyComponent::CreateRigidBodyBind( lua );
	FollowCamera::CreateLuaFollowCamera( lua, registry );
	ContactListenerBind::CreateLuaContactListener( lua, registry.GetRegistry() );
	SensorListenerBind::CreateLuaContactListener( lua, registry.GetRegistry() );
	OpenALSoundComponent::CreateLuaOpenALSoundComponentBind( lua );

	Entity::RegisterMetaComponent<TransformComponent>();
	Entity::RegisterMetaComponent<SpriteComponent>();
	Entity::RegisterMetaComponent<AnimationComponent>();
	Entity::RegisterMetaComponent<BoxColliderComponent>();
	Entity::RegisterMetaComponent<CircleColliderComponent>();
	Entity::RegisterMetaComponent<PhysicsComponent>();
	// Entity::RegisterMetaComponent<SoundEmitter>();
	// Entity::RegisterMetaComponent<SoundListener>();
	Entity::RegisterMetaComponent<TextComponent>();
	Entity::RegisterMetaComponent<RigidBodyComponent>();
	Entity::RegisterMetaComponent<OpenALSoundComponent>();

	Registry::RegisterMetaComponent<TransformComponent>();
	Registry::RegisterMetaComponent<SpriteComponent>();
	Registry::RegisterMetaComponent<AnimationComponent>();
	Registry::RegisterMetaComponent<BoxColliderComponent>();
	Registry::RegisterMetaComponent<CircleColliderComponent>();
	Registry::RegisterMetaComponent<PhysicsComponent>();
	// Registry::RegisterMetaComponent<SoundEmitter>();
	// Registry::RegisterMetaComponent<SoundListener>();
	Registry::RegisterMetaComponent<TextComponent>();
	Registry::RegisterMetaComponent<RigidBodyComponent>();
	Registry::RegisterMetaComponent<OpenALSoundComponent>();

	UserDataBinder::CreateLuaUserData( lua );
	UserDataBinder::register_meta_user_data<ObjectData>();

	State::CreateLuaStateBind( lua );
	StateStack::CreateLuaStateStackBind( lua );
	StateMachine::CreateLuaStateMachine( lua );
	StateSubMachine::CreateLuaStateSubMachine( lua );
}

void ScriptingSystem::RegisterLuaFunctions( sol::state& lua, Registry& registry )
{
	lua.set_function(
		"run_script", [&]( const std::string& path ) {
			try
			{
				lua.safe_script_file( path );
			}
			catch ( const sol::error& error )
			{
				LOG_ERROR( "Error loading Lua Script: {}", error.what() );
				return false;
			}
			return true;
		} );

	lua.set_function( "load_script_table", [&]( const sol::table& scriptList ) {
		if ( !scriptList.valid() )
		{
			LOG_ERROR( "Failed to load script list: Table is invalid." );
			return;
		}

		for ( const auto& [index, script] : scriptList )
		{
			try
			{
				auto result = lua.safe_script_file( script.as<std::string>() );
				if ( !result.valid() )
				{
					sol::error error = result;
					throw error;
				}
			}
			catch ( const sol::error& error )
			{
				LOG_ERROR( "Failed to load script: {}, Error: {}", script.as<std::string>(), error.what() );
				return;
			}
		}
	} );

	lua.set_function( "get_ticks", [] {
		return glfwGetTime();
	} );

	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();
	lua.set_function( "measure_text", [&]( const std::string& text, const std::string& fontName ) {
		const auto& pFont = assetManager.GetFont( fontName );
		if ( !pFont )
		{
			LOG_ERROR( "Failed to get font [{}] - Does not exist in asset manager!", fontName );
			return -1.f;
		}

		glm::vec2 position{ 0.f }, temp_pos{ position };
		for ( const auto& character : text )
			pFont->GetNextCharPos( character, temp_pos );

		return std::abs( ( position - temp_pos ).x );
	} );

	auto& engine = CoreEngineData::GetInstance();
	lua.set_function( "GetDeltaTime", [&] { return engine.GetDeltaTime(); } );
	lua.set_function( "WindowWidth", [&] { return engine.WindowWidth(); } );
	lua.set_function( "WindowHeight", [&] { return engine.WindowHeight(); } );
	lua.set_function( "DisablePhysics", [&] { engine.DisablePhysics(); } );
	lua.set_function( "EnablePhysics", [&] { engine.EnablePhysics(); } );
	lua.set_function( "IsPhysicsEnabled", [&] { return engine.IsPhysicsEnabled(); } );

	// Render Colliders Enable functions
	lua.set_function( "DisableRenderColliders", [&] { engine.DisableColliderRender(); } );
	lua.set_function( "EnableRenderColliders", [&] { engine.EnableColliderRender(); } );
	lua.set_function( "IsRenderCollidersEnabled", [&] { return engine.RenderCollidersEnabled(); } );

	lua.new_usertype<RandomGenerator>(
		"Random",
		sol::call_constructor,
		sol::constructors<RandomGenerator( uint32_t, uint32_t ), RandomGenerator()>(),
		"get_float", &RandomGenerator::GetFloat,
		"get_int", &RandomGenerator::GetInt );
}
