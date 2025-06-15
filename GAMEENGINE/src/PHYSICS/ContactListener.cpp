#include "pch.h"

#include "PHYSICS/ContactListener.h"

#include "PHYSICS/Box2DWrappers.h"
#include "PHYSICS/UserData.h"

void ContactListener::SetUserContacts( UserData* a, UserData* b )
{
	m_pUserDataA = a;
	m_pUserDataB = b;
}

void ContactListener::BeginContact( b2WorldId myWorldId )
{
	b2ContactEvents contactEvents = b2World_GetContactEvents( myWorldId );
	for ( int i = 0; i < contactEvents.beginCount; ++i )
	{
		b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;
		UserData* a_data =
			reinterpret_cast<UserData*>(
				b2Shape_GetUserData( beginEvent->shapeIdA ) );
		UserData* b_data =
			reinterpret_cast<UserData*>(
				b2Shape_GetUserData( beginEvent->shapeIdB ) );

		if ( !a_data || !b_data )
			return; // Add this because it caused runtime error after create Play/Stop Scenes.[EP: 57].

		try
		{
			auto* a_any = std::any_cast<ObjectData>( &a_data->userData );
			auto* b_any = std::any_cast<ObjectData>( &b_data->userData );

			a_any->AddContact( b_any );
			b_any->AddContact( a_any );

			SetUserContacts( a_data, b_data );
		}
		catch ( const std::bad_any_cast& ex )
		{
			LOG_ERROR( "Failed to cast user contacts: {}", ex.what() );
			SetUserContacts( nullptr, nullptr );
		}
	}
}

void ContactListener::EndContact( b2WorldId myWorldId )
{
	b2ContactEvents contactEvents = b2World_GetContactEvents( myWorldId );
	for ( int i = 0; i < contactEvents.endCount; ++i )
	{
		b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;
		UserData* a_data =
			reinterpret_cast<UserData*>(
				b2Shape_GetUserData( endEvent->shapeIdA ) );
		UserData* b_data =
			reinterpret_cast<UserData*>(
				b2Shape_GetUserData( endEvent->shapeIdB ) );

		if ( !a_data || !b_data )
			return; // Add this because it caused runtime error after create Play/Stop Scenes.[EP: 57].

		try
		{
			// a_data->userData is the std::any itself
			// To call the overload any_cast<T>(std::any*), you must pass the address of that std::any object.
			auto* a_any = std::any_cast<ObjectData>( &( a_data->userData ) );
			auto* b_any = std::any_cast<ObjectData>( &( b_data->userData ) );

			a_any->RemoveContact( b_any );
			b_any->RemoveContact( a_any );
		}
		catch ( const std::bad_any_cast& ex )
		{
			LOG_ERROR( "Failed to cast user contacts: {}", ex.what() );
		}

		// Use b2Shape_IsValid because a shape may have been destroyed
		if ( b2Shape_IsValid( endEvent->shapeIdA ) && b2Shape_IsValid( endEvent->shapeIdB ) )
		{
			// ShapesStopTouching(endEvent->shapeIdA, endEvent->shapeIdB);
			SetUserContacts( nullptr, nullptr );
		}
	}
}

void ContactListener::ResetUserContactsAB()
{
	SetUserContacts( nullptr, nullptr );
}
