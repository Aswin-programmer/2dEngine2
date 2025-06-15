#include "pch.h"

#include "ECS/MainRegistry.h"
#include "LOGGER/log.h"
#include "RESOURCES/AssetManager.h"

MainRegistry& MainRegistry::GetInstance()
{
	static MainRegistry instance{};
	return instance;
}

void MainRegistry::Initialize()
{
	m_pMainRegistry = std::make_unique<Registry>();
	assert( m_pMainRegistry && "Failed to initialize main registry." );

	auto pAssetManager = std::make_shared<AssetManager>();
	if ( !pAssetManager )
	{
		LOG_ERROR( "Failed to create the asset manager" );
		m_bInitialized = false;
		return;
	}
	m_pMainRegistry->AddToContext<std::shared_ptr<AssetManager>>( std::move( pAssetManager ) );

	m_bInitialized = true;
}

AssetManager& MainRegistry::GetAssetManager()
{
	assert( m_bInitialized && "Main Registry must be initialized before use." );
	return *m_pMainRegistry->GetContext<std::shared_ptr<AssetManager>>();
}
