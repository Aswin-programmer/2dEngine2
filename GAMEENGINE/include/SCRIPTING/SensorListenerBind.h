#pragma once

class SensorListener;

class SensorListenerBind
{
private:
	static sol::table GetUserData( SensorListener& sensorListener, sol::this_state s );

public:
	static void CreateLuaContactListener( sol::state& lua, entt::registry& registry );
};
