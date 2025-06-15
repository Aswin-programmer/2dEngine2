#include "pch.h"

#include "PHYSICS/Box2DWrappers.h"

Box2DWrappers::Box2DWrappers()
	: isBox2dLoaded{ false },
	  worldId( b2_nullWorldId )
{
}

Box2DWrappers::~Box2DWrappers()
{
	if ( isBox2dLoaded )
	{
		b2DestroyWorld( worldId );
	}
}

void Box2DWrappers::LoadBox2dWorld()
{
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0.0f, -10.0f };
	worldId = b2CreateWorld( &worldDef );
	if ( !b2World_IsValid( worldId ) )
	{
		LOG_ERROR( "Failed to create the Box2D world [{0}]", worldId.index1 );
	}
	isBox2dLoaded = true;
}

void Box2DWrappers::UnLoadBox2dWorld()
{
	b2DestroyWorld( worldId );
	isBox2dLoaded = false;
}
