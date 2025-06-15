#pragma once

class ContactListener;

class ContactListenerBind
{
private:
	static std::tuple<sol::object, sol::object> GetUserData( ContactListener& contactListener, sol::this_state s );

public:
	static void CreateLuaContactListener( sol::state& lua, entt::registry& registry );
};
