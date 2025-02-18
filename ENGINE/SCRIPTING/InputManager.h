#pragma once
#pragma once
#include "../WINDOW/INPUT/Keyboard.h"
#include <memory>
#include <sol/sol.hpp>

class InputManager
{
private:
	std::unique_ptr<Keyboard> m_pKeyboard;
private:
	InputManager();
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
private:
	static void RegisterLuaKeyNames(sol::state& lua);
public:
	static InputManager& GetInstance();
	static void CreateLuaInputBindings(sol::state& lua);
	inline Keyboard& GetKeyboard() { return *m_pKeyboard; }
};