#include "pch.h"

#include "SCRIPTING/InputManager.h"

#include "ECS/Registry.h"
#include "RENDERER/CORE/Camera2D.h"
#include "WINDOW/INPUT/Button.h"
#include "WINDOW/INPUT/Keys.h"
#include "WINDOW/INPUT/Keyboard.h"
#include "WINDOW/INPUT/Mouse.h"

InputManager::InputManager()
	: m_pKeyboard{ std::make_unique<Keyboard>() },
	  m_pMouse{ std::make_unique<Mouse>() }
{
}

void InputManager::RegisterLuaKeyNames( sol::state& lua )
{
	// ==================================================================
	// Register Typewriter Keys
	// ==================================================================
	lua.set( "KEY_A", KEY_A );
	lua.set( "KEY_B", KEY_B );
	lua.set( "KEY_C", KEY_C );
	lua.set( "KEY_D", KEY_D );
	lua.set( "KEY_E", KEY_E );
	lua.set( "KEY_F", KEY_F );
	lua.set( "KEY_G", KEY_G );
	lua.set( "KEY_H", KEY_H );
	lua.set( "KEY_I", KEY_I );
	lua.set( "KEY_J", KEY_J );
	lua.set( "KEY_K", KEY_K );
	lua.set( "KEY_L", KEY_L );
	lua.set( "KEY_M", KEY_M );
	lua.set( "KEY_N", KEY_N );
	lua.set( "KEY_O", KEY_O );
	lua.set( "KEY_P", KEY_P );
	lua.set( "KEY_Q", KEY_Q );
	lua.set( "KEY_R", KEY_R );
	lua.set( "KEY_S", KEY_S );
	lua.set( "KEY_T", KEY_T );
	lua.set( "KEY_U", KEY_U );
	lua.set( "KEY_V", KEY_V );
	lua.set( "KEY_W", KEY_W );
	lua.set( "KEY_X", KEY_X );
	lua.set( "KEY_Y", KEY_Y );
	lua.set( "KEY_Z", KEY_Z );
	lua.set( "KEY_0", KEY_0 );
	lua.set( "KEY_1", KEY_1 );
	lua.set( "KEY_2", KEY_2 );
	lua.set( "KEY_3", KEY_3 );
	lua.set( "KEY_4", KEY_4 );
	lua.set( "KEY_5", KEY_5 );
	lua.set( "KEY_6", KEY_6 );
	lua.set( "KEY_7", KEY_7 );
	lua.set( "KEY_8", KEY_8 );
	lua.set( "KEY_9", KEY_9 );
	lua.set( "KEY_ENTER", KEY_RETURN );
	lua.set( "KEY_BACKSPACE", KEY_BACKSPACE );
	lua.set( "KEY_ESC", KEY_ESCAPE );
	lua.set( "KEY_SPACE", KEY_SPACE );
	lua.set( "KEY_LCTRL", KEY_LCTRL );
	lua.set( "KEY_RCTRL", KEY_RCTRL );
	lua.set( "KEY_LALT", KEY_LALT );
	lua.set( "KEY_RALT", KEY_RALT );
	lua.set( "KEY_LSHIFT", KEY_LSHIFT );
	lua.set( "KEY_RSHIFT", KEY_RSHIFT );
	// ==================================================================
	//  Register Function Keys
	// ==================================================================
	lua.set( "KEY_F1", KEY_F1 );
	lua.set( "KEY_F2", KEY_F2 );
	lua.set( "KEY_F3", KEY_F3 );
	lua.set( "KEY_F4", KEY_F4 );
	lua.set( "KEY_F5", KEY_F5 );
	lua.set( "KEY_F6", KEY_F6 );
	lua.set( "KEY_F7", KEY_F7 );
	lua.set( "KEY_F8", KEY_F8 );
	lua.set( "KEY_F9", KEY_F9 );
	lua.set( "KEY_F10", KEY_F10 );
	lua.set( "KEY_F11", KEY_F11 );
	lua.set( "KEY_F12", KEY_F12 );
	// ==================================================================
	// Register Cursor Control Keys
	// ==================================================================
	lua.set( "KEY_UP", KEY_UP );
	lua.set( "KEY_RIGHT", KEY_RIGHT );
	lua.set( "KEY_DOWN", KEY_DOWN );
	lua.set( "KEY_LEFT", KEY_LEFT );
	// ==================================================================
	// Register Numeric Keypad Keys
	// ==================================================================
	lua.set( "KP_KEY_0", KEY_KP0 );
	lua.set( "KP_KEY_1", KEY_KP1 );
	lua.set( "KP_KEY_2", KEY_KP2 );
	lua.set( "KP_KEY_3", KEY_KP3 );
	lua.set( "KP_KEY_4", KEY_KP4 );
	lua.set( "KP_KEY_5", KEY_KP5 );
	lua.set( "KP_KEY_6", KEY_KP6 );
	lua.set( "KP_KEY_7", KEY_KP7 );
	lua.set( "KP_KEY_8", KEY_KP8 );
	lua.set( "KP_KEY_9", KEY_KP9 );
	lua.set( "KP_KEY_ENTER", KEY_KP_ENTER );
}

void InputManager::RegisterMouseBtnNames( sol::state& lua )
{
	lua.set( "LEFT_BTN", MOUSE_LEFT );
	lua.set( "MIDDLE_BTN", MOUSE_MIDDLE );
	lua.set( "RIGHT_BTN", MOUSE_RIGHT );
}

InputManager& InputManager::GetInstance()
{
	static InputManager instance{};
	return instance;
}

void InputManager::CreateLuaInputBindings( sol::state& lua, Registry& registry )
{
	RegisterLuaKeyNames( lua );
	RegisterMouseBtnNames( lua );

	auto& inputManager = GetInstance();
	auto& keyboard = inputManager.GetKeyboard();
	auto& camera = registry.GetContext<std::shared_ptr<Camera2D>>();

	lua.new_usertype<Keyboard>(
		"Keyboard",
		sol::no_constructor,
		"just_pressed", [&]( int key ) { return keyboard.IsKeyJustPressed( key ); },
		"just_released", [&]( int key ) { return keyboard.IsKeyJustReleased( key ); },
		"pressed", [&]( int key ) { return keyboard.IsKeyPressed( key ); } );

	auto& mouse = inputManager.GetMouse();

	lua.new_usertype<Mouse>(
		"Mouse",
		sol::no_constructor,
		"just_pressed", [&]( int btn ) { return mouse.IsBtnJustPressed( btn ); },
		"just_released", [&]( int btn ) { return mouse.IsBtnJustReleased( btn ); },
		"pressed", [&]( int btn ) { return mouse.IsBtnPressed( btn ); },
		"screen_position", [&]() {
			auto [x, y] = mouse.GetMouseScreenPosition();
			return glm::vec2{ x, y }; },
		"world_position", [&]() {
			auto [x, y] = mouse.GetMouseScreenPosition();
			return camera->ScreenCoordsToWorld(glm::vec2{ x, y }); },
		"wheel_x", [&]() { return mouse.GetMouseWheelX(); },
		"wheel_y", [&]() { return mouse.GetMouseWheelY(); } );
}
