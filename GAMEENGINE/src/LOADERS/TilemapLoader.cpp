#include "pch.h"

#include "LOADERS/TilemapLoader.h"

#include "ECS/Entity.h"
#include "ECS/COMPONENTS/AllComponents.h"
#include "ECS/ComponentSerializer.h"
#include "FILESYSTEM/SERIALIZERS/JSONSerializer.h"
#include "LOGGER/Log.h"

bool TilemapLoader::SaveTilemapJSON( Registry& registry, const std::string& sTilemapFile )
{
	std::unique_ptr<JSONSerializer> pSerializer{ nullptr };
	try
	{
		pSerializer = std::make_unique<JSONSerializer>( sTilemapFile );
	}
	catch ( const std::exception& ex )
	{
		LOG_ERROR( "Failed to save tilemap [{}] - [{}]", sTilemapFile, ex.what() );
		return false;
	}

	std::filesystem::path tilemapPath{ sTilemapFile };
	if ( !std::filesystem::exists( tilemapPath ) )
	{
		LOG_ERROR( "Failed to save tilemap - Filepath does not exists [{}]", sTilemapFile );
		return false;
	}

	pSerializer->StartDocument();
	pSerializer->StartNewArray( "tilemap" );

	auto tiles = registry.GetRegistry().view<TileComponent>();
	for ( auto tile : tiles )
	{
		pSerializer->StartNewObject();
		pSerializer->StartNewObject( "components" );
		auto tileEnt{ Entity{ registry, tile } };

		auto& transform = tileEnt.GetComponent<TransformComponent>();
		SERIALIZE_COMPONENT( *pSerializer, transform );

		auto& sprite = tileEnt.GetComponent<SpriteComponent>();
		SERIALIZE_COMPONENT( *pSerializer, sprite );

		if ( tileEnt.HasComponent<BoxColliderComponent>() )
		{
			auto& boxcollider = tileEnt.GetComponent<BoxColliderComponent>();
			SERIALIZE_COMPONENT( *pSerializer, boxcollider );
		}

		if ( tileEnt.HasComponent<CircleColliderComponent>() )
		{
			auto& circlecollider = tileEnt.GetComponent<CircleColliderComponent>();
			SERIALIZE_COMPONENT( *pSerializer, circlecollider );
		}

		if ( tileEnt.HasComponent<AnimationComponent>() )
		{
			auto& animation = tileEnt.GetComponent<AnimationComponent>();
			SERIALIZE_COMPONENT( *pSerializer, animation );
		}

		if ( tileEnt.HasComponent<PhysicsComponent>() )
		{
			auto& physics = tileEnt.GetComponent<PhysicsComponent>();
			SERIALIZE_COMPONENT( *pSerializer, physics );
		}

		pSerializer->EndNewObject(); // Component object
		pSerializer->EndNewObject(); // tile object
	}

	pSerializer->EndNewArray(); // Tilemap array

	return pSerializer->EndDocument();
}

bool TilemapLoader::LoadTilemapJSON( Registry& registry, const std::string& sTilemapFile )
{
	return false;
}

bool TilemapLoader::SaveTilemap( Registry& registry, const std::string& sTilemapFile, bool bUseJSON )
{
	if ( bUseJSON )
	{
		return SaveTilemapJSON( registry, sTilemapFile );
	}

	return false;
}

bool TilemapLoader::LoadTilemap( Registry& registry, const std::string& sTilemapFile, bool bUseJSON )
{
	if ( bUseJSON )
	{
		return LoadTilemapJSON( registry, sTilemapFile );
	}

	return false;
}
