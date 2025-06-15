#pragma once

#include "LOGGER/log.h"
#include "SensorListener.h"

struct BodyWrapper
{
	b2BodyId bodyId{ b2_nullBodyId };
	BodyWrapper( const b2BodyId& bId ) : bodyId{ bId } {}
};

struct BodyDestroyer
{
	void operator()( BodyWrapper* body ) const
	{
		if ( b2Body_IsValid( body->bodyId ) )
		{
			auto& sensorMapRef = SensorListener::sensorData;
			LUA_INFO( "The size of sensor contact vector is : {}", sensorMapRef.size() );

			for ( auto it = sensorMapRef.begin(); it != sensorMapRef.end(); )
			{
				const auto& [bodyA, bodyB] = it->first;
				if ( bodyA.index1 == body->bodyId.index1 || bodyB.index1 == body->bodyId.index1 )
				{
					LOG_INFO( "Removing sensor contact data for body {} and body {}", bodyA.index1, bodyB.index1 );
					// LOG_INFO("Removing sensor contact data for body {}", body->bodyId.index1);
					it = sensorMapRef.erase( it ); // Use the returned iterator
				}
				else
				{
					++it;
				}
			}

			//=>A similar thing for the contact listener has to be implemented!

			b2DestroyBody( body->bodyId );
			LOG_INFO( "Body Destroyed!" );
		}
	}
};

static std::shared_ptr<BodyWrapper> MakeSharedBody( b2WorldId worldId, b2BodyDef* bodyDef )
{
	return std::shared_ptr<BodyWrapper>( new BodyWrapper( b2CreateBody( worldId, bodyDef ) ), BodyDestroyer{} );
}

class Box2DWrappers
{
public:
	Box2DWrappers();
	~Box2DWrappers();

	void LoadBox2dWorld();
	void UnLoadBox2dWorld();

	b2WorldId GetWorldID() const { return worldId; }

private:
	bool isBox2dLoaded;
	b2WorldId worldId;
};
