#include "pch.h"

#include "SCRIPTING/SensorListenerBind.h"

#include "ECS/MetaUtilities.h"
#include "PHYSICS/SensorListener.h"
#include "PHYSICS/UserData.h"

// std::tuple<sol::object, sol::object> SensorListenerBind::GetUserData(SensorListener& sensorListener, sol::this_state s)
//{
//     /*auto pUserSensorDataA = sensorListener.GetUserSensorDataA();
//     auto pUserSensorDataB = sensorListener.GetUserSensorDataB();
//
//     if (!pUserSensorDataA || !pUserSensorDataB)
//         return std::make_tuple(sol::lua_nil_t{}, sol::lua_nil_t{});
//
//     assert(pUserSensorDataA->type_id != 0 && pUserSensorDataB->type_id != 0 && "User Data Type id must be set!");
//
//     using namespace entt::literals;
//
//     const auto maybe_any_a = InvokeMetaFunction(
//         static_cast<entt::id_type>(pUserSensorDataA->type_id),
//         "get_user_data"_hs,
//         *pUserSensorDataA, s
//     );
//
//     const auto maybe_any_b = InvokeMetaFunction(
//         static_cast<entt::id_type>(pUserSensorDataB->type_id),
//         "get_user_data"_hs,
//         *pUserSensorDataB, s
//     );
//
//     if (!maybe_any_a || !maybe_any_b)
//         return std::make_tuple(sol::lua_nil_t{}, sol::lua_nil_t{});
//
//     return std::make_tuple(
//         maybe_any_a.cast<sol::reference>(),
//         maybe_any_b.cast<sol::reference>()
//     );*/
//
//     }
//
// }

sol::table SensorListenerBind::GetUserData( SensorListener& sensorListener, sol::this_state s )
{
	sol::state_view lua( s );
	sol::table contacts_table = lua.create_table();

	for ( const auto& entry : SensorListener::sensorData )
	{
		UserData* a_data = entry.second.first;
		UserData* b_data = entry.second.second;

		if ( !a_data || !b_data )
			continue;

		using namespace entt::literals;

		assert( a_data->type_id != 0 && b_data->type_id != 0 && "User Data Type id must be set!" );

		const auto maybe_any_a = InvokeMetaFunction(
			static_cast<entt::id_type>( a_data->type_id ),
			"get_user_data"_hs,
			*a_data, s );

		const auto maybe_any_b = InvokeMetaFunction(
			static_cast<entt::id_type>( b_data->type_id ),
			"get_user_data"_hs,
			*b_data, s );

		if ( !maybe_any_a || !maybe_any_b )
			continue;

		sol::table pair_table = lua.create_table();
		pair_table[1] = maybe_any_a.cast<sol::reference>();
		pair_table[2] = maybe_any_b.cast<sol::reference>();

		contacts_table.add( pair_table );
	}

	return contacts_table;
}

void SensorListenerBind::CreateLuaContactListener( sol::state& lua, entt::registry& registry )
{
	auto& sensorListener = registry.ctx().get<std::shared_ptr<SensorListener>>();
	if ( !sensorListener )
	{
		LOG_ERROR( "Failed to create the sensor listener lua bind - Sensor listener is not in the registry!" );
		return;
	}

	lua.new_usertype<SensorListener>(
		"SensorListener",
		sol::no_constructor,
		"get_user_data", [&]( sol::this_state s ) {
			return GetUserData( *sensorListener, s );
		} );
}
