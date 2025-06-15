#include "pch.h"

#include "PHYSICS/SensorListener.h"

std::map<std::pair<b2BodyId, b2BodyId>, std::pair<UserData*, UserData*>, BodyPairComparator> SensorListener::sensorData;

void SensorListener::SetUserSensorContacts( UserData* a, UserData* b )
{
	m_pUserSensorDataA = a;
	m_pUserSensorDataB = b;
}

void SensorListener::ProcessSensorContactEvents( b2WorldId myWorldId )
{
	b2SensorEvents sensorEvents = b2World_GetSensorEvents( myWorldId );
	for ( int i = 0; i < sensorEvents.beginCount; ++i )
	{
		b2SensorBeginTouchEvent* beginTouch = sensorEvents.beginEvents + i;
		if ( b2Shape_IsValid( beginTouch->sensorShapeId ) && b2Shape_IsValid( beginTouch->visitorShapeId ) )
		{
			UserData* a_data = reinterpret_cast<UserData*>(
				b2Shape_GetUserData( beginTouch->sensorShapeId ) );
			UserData* b_data = reinterpret_cast<UserData*>(
				b2Shape_GetUserData( beginTouch->visitorShapeId ) );
			if ( a_data && b_data )
			{
				// SetUserSensorContacts(a_data, b_data);
				//  Get body IDs from the shape-to-body map
				b2BodyId bodyA = b2Shape_GetBody( beginTouch->sensorShapeId );
				b2BodyId bodyB = b2Shape_GetBody( beginTouch->visitorShapeId );

				// Use sorted body pair to ensure consistent key ordering
				std::pair<b2BodyId, b2BodyId> key = ( bodyA.index1 < bodyB.index1 )
														? std::make_pair( bodyA, bodyB )
														: std::make_pair( bodyB, bodyA );

				// Store the user data
				sensorData[key] = std::make_pair( a_data, b_data );
				// LUA_INFO("SENSOR EVENT BEGIN");
			}
		}
	}

	for ( int i = 0; i < sensorEvents.endCount; ++i )
	{
		b2SensorEndTouchEvent* endTouch = sensorEvents.endEvents + i;
		if ( b2Shape_IsValid( endTouch->sensorShapeId ) && b2Shape_IsValid( endTouch->visitorShapeId ) )
		{
			UserData* a_data = reinterpret_cast<UserData*>(
				b2Shape_GetUserData( endTouch->sensorShapeId ) );
			UserData* b_data = reinterpret_cast<UserData*>(
				b2Shape_GetUserData( endTouch->visitorShapeId ) );

			if ( a_data && b_data )
			{
				// Get body IDs from shape-to-body map
				b2BodyId bodyA = b2Shape_GetBody( endTouch->sensorShapeId );
				b2BodyId bodyB = b2Shape_GetBody( endTouch->visitorShapeId );

				// Construct a sorted key (same order as BeginSensorContact)
				std::pair<b2BodyId, b2BodyId> key = ( bodyA.index1 < bodyB.index1 )
														? std::make_pair( bodyA, bodyB )
														: std::make_pair( bodyB, bodyA );

				// Remove from the map if it exists
				sensorData.erase( key );

				// SetUserSensorContacts(nullptr, nullptr);
				// LUA_INFO("SENSOR EVENT END");
			}
		}
	}
}

void SensorListener::ResetUserSensorAB()
{
	SetUserSensorContacts( nullptr, nullptr );
}
